/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:55:33 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/02 17:38:06 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *read_full_command_line(char *first_line)
{
    char *acc = first_line;

    while (1) {
        t_more need = need_more_input(acc);
        if (need == MORE_NONE) {
            if (*acc && !is_blank_line(acc))
                add_history(acc);
            return acc;
        }

        const char *ps2 = get_ps2_prompt(need);
        char *next = readline(ps2);
        if (!next) {
            free(acc);
            if (handle_unexpected_eof(need) == -1)
                return NULL;
        }

        acc = append_line(acc, next);
        free(next);
        if (!acc)
            return NULL;
    }
}
