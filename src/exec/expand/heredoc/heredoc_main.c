/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 17:49:05 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/09 13:32:44 by tkuwahat         ###   ########.fr       */
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

static int	read_one_heredoc(const char *delim, int quoted, int *out_fd)
{
	int		pfd[2];
	pid_t	pid;

	if (!delim || !out_fd)
		return (-1);
	*out_fd = -1;
	if (pipe(pfd) < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		close(pfd[0]);
		close(pfd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(pfd[0]);
		run_heredoc_child(delim, quoted, pfd[1]);
	}
	close(pfd[1]);
	return (wait_heredoc_child(pid, pfd[0], out_fd));
}

static int	handle_one_heredoc(t_redir *r)
{
	int	rc;

	if (heredoc_build_delim_inplace(r) < 0)
		return (-1);
	rc = read_one_heredoc(r->path, r->quoted_heredoc, &r->hdoc_fd);
	free(r->path);
	r->path = NULL;
	return (rc);
}

int	collect_heredocs(t_cmd *c)
{
	size_t	i;
	int		rc;

	if (!c || !c->redirs)
		return (0);
	i = 0;
	while (i < c->n_redirs)
	{
		if (c->redirs[i].kind == TOK_HEREDOC)
		{
			rc = handle_one_heredoc(&c->redirs[i]);
			if (rc != 0)
				return (rc);
		}
		i++;
	}
	return (0);
}
