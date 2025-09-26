/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 15:40:21 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/24 20:45:55 by nnishiya         ###   ########.fr       */
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
        if (ast)
        {
            print_ast(ast, 0);
            destroy(ast);
            free_tokens(tokens);
        }
        // dump_tokens(tokens);
        free(full);
    }
    return 0;
}
