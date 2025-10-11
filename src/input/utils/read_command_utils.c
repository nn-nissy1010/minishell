/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:55:33 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/11 09:34:11 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*get_ps2_prompt(t_more need)
{
	if (need == MORE_OP)
		return ("> ");
	if (need == MORE_QUOTE_S)
		return ("quote> ");
	if (need == MORE_QUOTE_D)
		return ("dquote> ");
	return ("");
}

int	handle_unexpected_eof(t_more need)
{
	if (need == MORE_QUOTE_S)
		print_syntax_error("unexpected EOF while looking for matching '\''");
	else if (need == MORE_QUOTE_D)
		print_syntax_error("unexpected EOF while looking for matching '\"'");
	else
		print_syntax_error("syntax error near unexpected token `newline'");
	set_exit_status(258);
	return (-1);
}

char	*append_line(char *acc, const char *next)
{
	char	*new_acc;

	new_acc = ft_strjoin_3word(acc, "\n", next);
	if (!new_acc)
	{
		print_syntax_error("allocation error");
		set_exit_status(1);
	}
	return (new_acc);
}
