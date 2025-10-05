/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:55:33 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/05 21:52:06 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *append_next_line(char *acc, t_more need)
{
    const char *ps2;
    char *next;
    char *tmp;

    ps2 = get_ps2_prompt(need);
    next = readline(ps2);
    if (!next)
        return NULL;
    tmp = append_line(acc, next);
    free(next);
    return tmp;
}

static int handle_eof_case(char *acc, t_more need)
{
    free(acc);
    if (handle_unexpected_eof(need) == -1)
        return -1;
    return 0;
}

char *read_full_command_line(char *first_line)
{
    char *acc;
    t_more need;

    acc = first_line;
    while (1)
    {
        need = need_more_input(acc);
        if (need == MORE_NONE)
        {
            if (*acc && !is_blank_line(acc))
                add_history(acc);
            return acc;
        }
        acc = append_next_line(acc, need);
        if (!acc)
        {
            if (handle_eof_case(acc, need) == -1)
                return NULL;
        }
    }
}
