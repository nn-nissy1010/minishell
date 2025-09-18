/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 15:40:21 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/18 11:20:46 by nnishiya         ###   ########.fr       */
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
        printf("入力: %s(ここで本来はコマンド処理する)\n", full);
        free(full);
    }
    return 0;
}
