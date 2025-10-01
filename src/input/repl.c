/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 15:40:21 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/30 18:06:23 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char *quote_name(int q)
{
    if (q == Q_NONE)   return "none";
    if (q == Q_SINGLE) return "single";
    if (q == Q_DOUBLE) return "double";
    return "???";
}

void dump_arg(const t_arg *arg)
{
    printf("ARG: raw=\"%s\"\n",
        arg->raw ? arg->raw : "(null)");

    for (t_arg_part *p = arg->parts; p; p = p->next) {
        printf("  part: text=\"%s\" quote=%s param=%d glob=%d\n",
            p->text ? p->text : "(null)",
            quote_name(p->quote),
            p->has_param,
            p->has_unq_glob);
    }
}

void dump_tokens(const t_token *toks)
{
    for (const t_token *t = toks; t; t = t->next) {
        switch (t->type) {
        case TOK_ARG:
            dump_arg(&t->u.arg);
            break;
        case TOK_PIPE:           printf("PIPE\n"); break;
        case TOK_REDIR_IN:       printf("REDIR_IN <\n"); break;
        case TOK_REDIR_OUT:      printf("REDIR_OUT >\n"); break;
        case TOK_REDIR_APPEND:   printf("REDIR_APPEND >>\n"); break;
        case TOK_HEREDOC:        printf("HEREDOC <<\n"); break;
        case TOK_AND_IF:         printf("AND_IF &&\n"); break;
        case TOK_OR_IF:          printf("OR_IF ||\n"); break;
        case TOK_LPAR:           printf("LPAR (\n"); break;
        case TOK_RPAR:           printf("RPAR )\n"); break;
        case TOK_EOF:            printf("EOF\n"); break;
        default:                 printf("UNKNOWN\n"); break;
        }
    }
}

static const char *redir_kind_str(t_tokentype type)
{
    switch (type) {
        case TOK_REDIR_IN:     return "<";
        case TOK_REDIR_OUT:    return ">";
        case TOK_REDIR_APPEND: return ">>";
        case TOK_HEREDOC:      return "<<";
        default:               return "?";
    }
}

/* indent helper */
static void append_indent(int depth)
{
    for (int i = 0; i < depth; i++)
        printf("  ");
}

/* quote type → string */
static const char *quote_type_str(t_quote_type q)
{
    if (q == Q_SINGLE) return "SINGLE";
    if (q == Q_DOUBLE) return "DOUBLE";
    return "NONE";
}

/* arg.parts 出力 */
static void print_arg_parts(const t_arg_part *p, int depth)
{
    while (p)
    {
        append_indent(depth);
        printf("part: text=\"%s\", quote=%s, has_param=%d, has_unq_glob=%d\n",
               p->text ? p->text : "(null)",
               quote_type_str(p->quote),
               p->has_param,
               p->has_unq_glob);
        p = p->next;
    }
}

/* AST 出力 */
void print_ast(const t_node *n, int depth)
{
    if (!n) return;

    append_indent(depth);

    switch (n->type)
    {
        case ND_COMMAND:
            printf("CMD\n");
            for (size_t i = 0; i < n->as.cmd.n_argv_tokens; i++) {
                t_token *tok = n->as.cmd.argv_tokens[i];
                if (tok) {
                    append_indent(depth+1);
                    printf("arg[%zu] raw = %s\n",
                           i, tok->u.arg.raw ? tok->u.arg.raw : "(null)");

                    if (tok->u.arg.parts)
                        print_arg_parts(tok->u.arg.parts, depth+2);
                }
            }
            for (size_t i = 0; i < n->as.cmd.n_redirs; i++) {
                t_redir *r = &n->as.cmd.redirs[i];
                append_indent(depth+1);
                printf("redir[%zu] %s (fd=%d) -> %s",
                       i, redir_kind_str(r->kind), r->fd,
                       (r->word && r->word->u.arg.raw) ? r->word->u.arg.raw : "(null)");

                if (r->kind == TOK_HEREDOC && r->quoted_heredoc)
                    printf(" (heredoc, quoted)");

                printf("\n");
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

    if (depth == 0)
        printf("\n");
}

int repl(void){
    t_token *tokens;
    t_node *ast;
    
    install_signal_handlers();
    while (1) {
        char *line = readline("myshell> ");
        if (!line) {
            printf("bye! exitcode : %d\n", get_exit_status());
            break;
        }
        if (is_blank_line(line)) {
            free(line);
            continue;
        }
        char *full = read_full_command_line(line);
        if (!full) {
            continue;
        }
        tokens = lexer(full);
        ast = parse(tokens);
        if (!ast)
        {
            free_tokens(tokens);
            free(full); 
            continue;
        }
        if (ast)
        {
            print_ast(ast, 0);
            destroy_ast(ast);
            free_tokens(tokens);
        }
        free(full);
    }
    return 0;
}
