/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 17:49:05 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/10 02:18:11 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
