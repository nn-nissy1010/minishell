/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 20:04:48 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/24 20:44:10 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static t_token *peek(t_parser *p)
// {
//     return p->cur;
// }


/* 再帰的にASTを表示 */
void print_ast(const t_node *n, int depth)
{
    if (!n) return;

    for (int i = 0; i < depth; i++)
        printf("  ");

    switch (n->type)
    {
        case ND_COMMAND:
            printf("CMD\n");
            for (size_t i = 0; i < n->as.cmd.n_argv_tokens; i++) {
                t_token *tok = n->as.cmd.argv_tokens[i];
                if (tok && tok->u.arg.raw) {
                    for (int j = 0; j < depth+1; j++) printf("  ");
                    printf("arg[%zu] = %s\n", i, tok->u.arg.raw);
                }
            }
            for (size_t i = 0; i < n->as.cmd.n_redirs; i++) {
                t_redir *r = &n->as.cmd.redirs[i];
                for (int j = 0; j < depth+1; j++) printf("  ");
                printf("redir[%zu] fd=%d -> %s",
                       i, r->fd,
                       (r->word && r->word->u.arg.raw) ? r->word->u.arg.raw : "(null)");
                if (r->quoted_heredoc)
                    printf(" (heredoc, quoted)");
                printf("\n");   // ← ★必ず改行
            }
            break;

        case ND_PIPE:
            printf("PIPE\n");
            print_ast(n->as.bin.left, depth+1);
            print_ast(n->as.bin.right, depth+1);
            break;

        case ND_AND_IF:
            printf("AND_IF\n");
            print_ast(n->as.bin.left, depth+1);
            print_ast(n->as.bin.right, depth+1);
            break;

        case ND_OR_IF:
            printf("OR_IF\n");
            print_ast(n->as.bin.left, depth+1);
            print_ast(n->as.bin.right, depth+1);
            break;

        case ND_SUBSHELL:
            printf("SUBSHELL\n");
            print_ast(n->as.subshell.body, depth+1);
            break;
    }

    // ルートノード（depth==0）の出力後に空行を入れるとさらに見やすい
    if (depth == 0)
        printf("\n");
}

/* 再帰的にASTを解放 */
void destroy(t_node *n)
{
    if (!n) return;

    switch (n->type)
    {
        case ND_COMMAND:
            // redirs 配列を解放
            free(n->as.cmd.redirs);

            // argv_tokens 配列を解放（トークン自体はlexerの管理なのでfree不要）
            free(n->as.cmd.argv_tokens);

            // argv を解放（exec準備でmallocしたら）
            if (n->as.cmd.argv) {
                for (size_t i = 0; i < n->as.cmd.argc; i++)
                    free(n->as.cmd.argv[i]);
                free(n->as.cmd.argv);
            }
            break;

        case ND_PIPE:
        case ND_AND_IF:
        case ND_OR_IF:
            destroy(n->as.bin.left);
            destroy(n->as.bin.right);
            break;

        case ND_SUBSHELL:
            destroy(n->as.subshell.body);
            break;
    }

    free(n);
}


static int match(t_parser *p, t_tokentype type)
{
    return (p->cur && p->cur->type == type);
}

static t_token *consume(t_parser *p)
{
    t_token *tok = p->cur;
    if (p->cur)
        p->cur = p->cur->next;
    return tok;
}

/* ---- ノード生成ヘルパ ---- */

static t_node *new_node(t_ntype type)
{
    t_node *n = malloc(sizeof(*n));
    if (!n)
        return NULL;
    n->type = type;
    n->fn = NULL; // 後で exec/destroy テーブルを差し込む
    memset(&n->as, 0, sizeof(n->as));
    return n;
}

/* ---- パーサ本体 ---- */

t_node *parse_expr(t_parser *p)
{
    return parse_and_or(p);
}

t_node *parse_and_or(t_parser *p)
{
    t_node *left = parse_pipeline(p);

    while (match(p, TOK_AND_IF) || match(p, TOK_OR_IF))
    {
        t_tokentype op = p->cur->type;
        consume(p);

        t_node *right = parse_pipeline(p);

        t_node *node = new_node(op == TOK_AND_IF ? ND_AND_IF : ND_OR_IF);
        node->as.bin.left = left;
        node->as.bin.right = right;
        left = node;
    }
    return left;
}

t_node *parse_pipeline(t_parser *p)
{
    t_node *left = parse_command(p);

    while (match(p, TOK_PIPE))
    {
        consume(p);
        t_node *right = parse_command(p);

        t_node *node = new_node(ND_PIPE);
        node->as.bin.left = left;
        node->as.bin.right = right;
        left = node;
    }
    return left;
}

t_node *parse_command(t_parser *p)
{
    if (match(p, TOK_LPAR))
        return parse_subshell(p);
    else
        return parse_simple_command(p);
}

t_node *parse_subshell(t_parser *p)
{
    consume(p); // '('
    t_node *body = parse_expr(p);
    if (!match(p, TOK_RPAR))
    {
        // エラー処理は後で
        return NULL;
    }
    consume(p); // ')'

    t_node *node = new_node(ND_SUBSHELL);
    node->as.subshell.body = body;
    return node;
}

t_node *parse_simple_command(t_parser *p)
{
    t_node *node = new_node(ND_COMMAND);
    if (!node)
        return NULL;

    // 初期化
    node->as.cmd.redirs = NULL;
    node->as.cmd.n_redirs = 0;
    node->as.cmd.argv_tokens = NULL;
    node->as.cmd.n_argv_tokens = 0;
    node->as.cmd.argv = NULL;
    node->as.cmd.argc = 0;

    // 引数・リダイレクトを読み続ける
    while (p->cur &&
           p->cur->type != TOK_PIPE &&
           p->cur->type != TOK_AND_IF &&
           p->cur->type != TOK_OR_IF &&
           p->cur->type != TOK_RPAR &&
           p->cur->type != TOK_EOF)
    {
        if (p->cur->type == TOK_ARG)
        {
            // argv_tokens に追加
            size_t n = node->as.cmd.n_argv_tokens;
            t_token **tmp = realloc(node->as.cmd.argv_tokens,
                                    sizeof(t_token*) * (n + 1));
            if (!tmp) {
                perror("realloc");
                destroy(node);
                return NULL;
            }
            node->as.cmd.argv_tokens = tmp;
            node->as.cmd.argv_tokens[n] = p->cur;
            node->as.cmd.n_argv_tokens++;
            consume(p);
        }
        else if (p->cur->type == TOK_REDIR_IN ||
                 p->cur->type == TOK_REDIR_OUT ||
                 p->cur->type == TOK_REDIR_APPEND ||
                 p->cur->type == TOK_HEREDOC)
        {
            t_tokentype redir_type = p->cur->type;
            consume(p); // <, >, >>, << を消費

            if (!p->cur || p->cur->type != TOK_ARG) {
                fprintf(stderr, "syntax error: expected filename/delimiter after redir\n");
                destroy(node);
                return NULL;
            }

            t_redir r;
            memset(&r, 0, sizeof(r));
            r.word = p->cur;
            r.fd   = (redir_type == TOK_REDIR_IN) ? 0 : 1;
            r.path = NULL;

            if (redir_type == TOK_HEREDOC) {
                // delimiter がクォートされていたかどうか
                if (p->cur->u.arg.parts &&
                    p->cur->u.arg.parts->quote != Q_NONE)
                    r.quoted_heredoc = 1;
            }

            size_t m = node->as.cmd.n_redirs;
            t_redir *tmp = realloc(node->as.cmd.redirs,
                                   sizeof(t_redir) * (m + 1));
            if (!tmp) {
                perror("realloc");
                destroy(node);
                return NULL;
            }
            node->as.cmd.redirs = tmp;
            node->as.cmd.redirs[m] = r;
            node->as.cmd.n_redirs++;

            consume(p); // ファイル名 or delimiter を消費
        }
        else {
            break; // 想定外トークンに遭遇
        }
    }

    return node;
}

// parser.c に置く実装
t_node *parse(t_token *toklist)
{
    t_parser p;
    t_node   *root;

    p.cur = toklist;
    root = parse_expr(&p);

    if (!match(&p, TOK_EOF))
    {
        fprintf(stderr, "syntax error near unexpected token\n");
        destroy(root);
        return NULL;
    }
    return root;
}
