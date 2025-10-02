/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:55:33 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/02 11:00:56 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void try_add_history(const char *line)
{
    if (*line && !is_blank_line(line))
        add_history(line);
}

static char *handle_eof_case(char *acc, t_more need)
{
    free(acc);
    if (handle_unexpected_eof(need) == -1)
        return NULL;
    return NULL;
}

static char *read_and_append_line(char *acc, t_more need)
{
    const char *ps2;
    char *next;
    char *tmp;
    
    ps2= get_ps2_prompt(need);
    next = readline(ps2);
    if (!next)
        return handle_eof_case(acc, need);
    tmp = append_line(acc, next);
    free(acc);
    free(next);
    return tmp;
}

char *read_full_command_line(char *first_line)
{
    t_more need;
    char *acc;
    
    acc = ft_strdup(first_line);
    free(first_line);
    while (1)
    {
        need = need_more_input(acc);
        if (need == MORE_NONE)
        {
            try_add_history(acc);
            return acc;
        }
        acc = read_and_append_line(acc, need);
        if (!acc)
            return NULL;
    }
}
