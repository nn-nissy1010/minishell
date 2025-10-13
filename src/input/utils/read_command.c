/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:55:33 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/13 21:50:42 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_unexpected_eof_error(t_more need)
{
	if (need == MORE_QUOTE_D)
	{
		write(STDERR_FILENO,
			"minishell: unexpected EOF while looking for matching `\"`\n",
			57);
		set_exit_status(2);
	}
	else if (need == MORE_QUOTE_S)
	{
		write(STDERR_FILENO,
			"minishell: unexpected EOF while looking for matching `'`\n",
			57);
		set_exit_status(2);
	}
	else if (need == MORE_OP)
	{
		write(STDERR_FILENO,
			"minishell: syntax error: unexpected end of file\n",
			48);
		set_exit_status(2);
	}
}

static char	*handle_more_input_exit(char *acc, t_more need, int is_eof)
{
	if (is_eof)
	{
		g_signal = 0;
		if (isatty(STDIN_FILENO))
		{
			print_unexpected_eof_error(need);
			free(acc);
			install_signal_handlers();
			return (ft_strdup(""));
		}
		return (NULL);
	}
	else if (g_signal == SIGINT)
	{
		write(STDOUT_FILENO, "^C\n", 3);
		free(acc);
		g_signal = 0;
		install_signal_handlers();
		return (ft_strdup(""));
	}
	return (NULL);
}

static char	*append_next_line(char *acc, t_more need)
{
	const char	*ps2;
	char		*next;
	char		*tmp;

	g_signal = 0;
	rl_done = 0;
	install_signal_handlers_more_input();
	ps2 = get_ps2_prompt(need);
	next = readline(ps2);

	/* EOF（Ctrl+D） */
	if (!next)
		return (handle_more_input_exit(acc, need, 1));

	/* Ctrl+C（SIGINT） */
	if (g_signal == SIGINT)
		return (handle_more_input_exit(acc, need, 0));

	/* 通常入力 */
	tmp = append_line(acc, next);
	free(next);
	install_signal_handlers();
	return (tmp);
}

static int	handle_eof_case(char *acc, t_more need)
{
	free(acc);
	if (handle_unexpected_eof(need) == -1)
		return (-1);
	return (0);
}

char	*read_full_command_line(char *first_line)
{
	char	*acc;
	t_more	need;

	acc = first_line;
	while (1)
	{
		need = need_more_input(acc);
		if (need == MORE_NONE)
		{
			if (*acc && !is_blank_line(acc))
				add_history(acc);
			return (acc);
		}
		acc = append_next_line(acc, need);
		if (!acc)
		{
			if (handle_eof_case(acc, need) == -1)
				return (NULL);
		}
	}
}
