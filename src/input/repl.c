/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 15:40:21 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/18 20:22:56 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int repl(void){
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
        t_token *ts = lexer(full);
        for (t_token *p = ts; p; p = p->next)
        {
            if (p->type == TOK_ARG)
                printf("ARG: raw=\"%s\"\n", p->u.arg.raw);
            else if (p->type == TOK_EOF)
                printf("EOF\n");
            else
                printf("SYM: type=%d\n", p->type);
        }
        free(full);
    }
    return 0;
}
