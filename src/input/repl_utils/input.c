/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:55:33 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/13 15:57:25 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char *get_ps2_prompt(t_more need)
{
    if (need == MORE_OP)
        return "> ";
    if (need == MORE_QUOTE_S)
        return "quote> ";
    if (need == MORE_QUOTE_D)
        return "dquote> ";
    return "";
}

static int handle_unexpected_eof(t_more need)
{
    if (need == MORE_QUOTE_S)
        print_syntax_error("unexpected EOF while looking for matching '\''");
    else if (need == MORE_QUOTE_D)
        print_syntax_error("unexpected EOF while looking for matching '\"'");
    else
        print_syntax_error("syntax error near unexpected token `newline'");
    g_exit_status = 258;
    return -1;
}

static char *append_line(char *acc, const char *next)
{
    char *new_acc = ft_strjoin_3word(acc, "\n", next);
    if (!new_acc) {
        print_syntax_error("allocation error");
        g_exit_status = 1;
    }
    return new_acc;
}

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
