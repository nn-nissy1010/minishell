/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 17:49:05 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/29 17:31:41 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	hdoc_process_line(char *line, int quoted, int p0, int p1)
{
	char	*exp;

	exp = hdoc_maybe_expand(line, quoted);
	free(line);
	if (!exp)
		return (hdoc_abort_sigint(p0, p1, NULL));
	if (hdoc_write_line(p1, exp) < 0)
	{
		free(exp);
		return (hdoc_abort_sigint(p0, p1, NULL));
	}
	free(exp);
	return (0);
}

static int	read_one_heredoc(const char *delim, int quoted, int *out_fd)
{
	int		pfd[2];
	char	*line;

	if (!delim || !out_fd)
		return (-1);
	if (hdoc_prepare(pfd) < 0)
		return (-1);
	while (1)
	{
		if (hdoc_should_abort(pfd[0], pfd[1]))
			return (-2);
		if (hdoc_read_line(&line, pfd[0], pfd[1]) != 0)
			return (-2);
		if (!line)
			break ;
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		if (hdoc_process_line(line, quoted, pfd[0], pfd[1]) != 0)
			return (-2);
	}
	return (hdoc_finish_success(pfd, out_fd));
}

static int	handle_one_heredoc(t_redir *r)
{
	int	rc;

	if (heredoc_build_delim_inplace(r) < 0)
		return (-1);
	rc = read_one_heredoc(r->path, r->quoted_heredoc, &r->fd);
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
