/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_run_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 17:49:05 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/13 18:48:16 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_heredoc_eof_warning(const char *delim, int lineno)
{
	ft_putstr_fd("minishell: warning: here-document at line ", 2);
	ft_putnbr_fd(lineno, 2);
	ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(delim, 2);
	ft_putendl_fd("')", 2);
}

static int	heredoc_readline_and_warn(const char *delim, int *lineno,
		char **out_line)
{
	char	*line;

	if (!lineno || !out_line)
		return (-1);
	line = readline("> ");
	*lineno += 1;
	if (!line)
	{
		print_heredoc_eof_warning(delim, *lineno);
		return (1);
	}
	*out_line = line;
	return (0);
}

static int	heredoc_process_line(const char *delim, int quoted, int write_fd,
		char *line)
{
	char	*exp;

	if (ft_strcmp(line, delim) == 0)
	{
		free(line);
		return (1);
	}
	exp = hdoc_maybe_expand(line, quoted);
	free(line);
	if (!exp)
		return (130);
	if (hdoc_write_line(write_fd, exp) < 0)
	{
		free(exp);
		return (130);
	}
	free(exp);
	return (0);
}

void	reset_heredoc_child_signals(void)
{
	struct sigaction	dfl;

	ft_memset(&dfl, 0, sizeof(dfl));
	dfl.sa_handler = SIG_DFL;
	sigemptyset(&dfl.sa_mask);
	dfl.sa_flags = 0;
	x_sigaction(SIGINT, &dfl);
	dfl.sa_handler = SIG_IGN;
	x_sigaction(SIGQUIT, &dfl);
}

void	run_heredoc_child(const char *delim, int quoted, int write_fd)
{
	char	*line;
	int		ln;
	int		rc;

	reset_heredoc_child_signals();
	bind_child_readline_to_tty();
	ln = 0;
	while (1)
	{
		rc = heredoc_readline_and_warn(delim, &ln, &line);
		if (rc != 0)
		{
			if (rc == 1)
				exit(0);
			exit(130);
		}
		rc = heredoc_process_line(delim, quoted, write_fd, line);
		if (rc == 1)
			exit(0);
		if (rc == 130)
			exit(130);
	}
}
