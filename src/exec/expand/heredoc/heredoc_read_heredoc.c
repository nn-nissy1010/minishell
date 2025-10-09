/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read_heredoc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 17:49:05 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/09 13:38:53 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	run_heredoc_child(const char *delim, int quoted, int write_fd)
{
	char	*line;
	char	*exp;

	reset_child_signals();
	bind_child_readline_to_tty();
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delim) == 0)
			exit(0);
		exp = hdoc_maybe_expand(line, quoted);
		free(line);
		if (!exp)
			exit(130);
		if (hdoc_write_line(write_fd, exp) < 0)
		{
			free(exp);
			exit(130);
		}
		free(exp);
	}
}

static int	wait_heredoc_child(pid_t pid, int read_fd, int *out_fd)
{
	int	st;

	if (waitpid(pid, &st, 0) < 0)
	{
		close(read_fd);
		return (-1);
	}
	if (WIFEXITED(st) && WEXITSTATUS(st) == 0)
	{
		*out_fd = read_fd;
		return (0);
	}
	close(read_fd);
	if ((WIFSIGNALED(st) && WTERMSIG(st) == SIGINT) || (WIFEXITED(st)
			&& WEXITSTATUS(st) == 130))
		return (-2);
	return (-1);
}

static int	heredoc_pipe_and_mask(int pfd[2], struct sigaction *oldint)
{
	if (!pfd || !oldint)
		return (-1);
	if (pipe(pfd) < 0)
		return (-1);
	parent_mask_sigint(oldint);
	return (0);
}

static int	heredoc_fork_child(const char *delim, int quoted, int pfd[2],
		pid_t *out_pid)
{
	pid_t	pid;

	if (!delim || !pfd || !out_pid)
		return (-1);
	*out_pid = -1;
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		close(pfd[0]);
		run_heredoc_child(delim, quoted, pfd[1]);
	}
	close(pfd[1]);
	*out_pid = pid;
	return (0);
}

int	read_one_heredoc(const char *delim, int quoted, int *out_fd)
{
	int					pfd[2];
	pid_t				pid;
	struct sigaction	oldint;
	int					rc;

	if (!delim || !out_fd)
		return (-1);
	*out_fd = -1;
	if (heredoc_pipe_and_mask(pfd, &oldint) < 0)
		return (-1);
	if (heredoc_fork_child(delim, quoted, pfd, &pid) < 0)
	{
		close(pfd[0]);
		close(pfd[1]);
		parent_unmask_sigint(&oldint);
		return (-1);
	}
	rc = wait_heredoc_child(pid, pfd[0], out_fd);
	parent_unmask_sigint(&oldint);
	return (rc);
}
