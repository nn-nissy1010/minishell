/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 22:09:18 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/02 19:37:42 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_heredoc_fds(t_cmd *c)
{
	size_t	i;

	if (!c || !c->redirs)
		return ;
	i = 0;
	while (i < c->n_redirs)
	{
		if (c->redirs[i].kind == TOK_HEREDOC && c->redirs[i].fd >= 0)
		{
			close(c->redirs[i].fd);
			c->redirs[i].fd = -1;
		}
		i++;
	}
}

static int	end_with_error(t_cmd *c, int status_set)
{
	if (status_set >= 0)
		set_exit_status(status_set);
	close_heredoc_fds(c);
	return (-1);
}

int	prepare_cmd_for_exec(t_cmd *c)
{
	int	rc;

	if (!c)
	{
		set_exit_status(2);
		return (-1);
	}
	rc = collect_heredocs(c);
	if (rc != 0)
	{
		if (rc == -2)
			return (end_with_error(c, 130));
		return (end_with_error(c, 1));
	}
	rc = expansion(c);
	if (rc != 0)
		return (end_with_error(c, -1));
	rc = glob_expand_argv(c);
	if (rc != 0)
		return (end_with_error(c, 1));
	return (0);
}

int	exec_cmd(t_node *node, t_exec_ctx *ctx)
{
	if (prepare_cmd_for_exec(&node->as.cmd) != 0)
		return (-1);
	return (exec_single(&node->as.cmd, ctx));
}

t_func	*v_cmd(void)
{
	static t_func	f = {exec_cmd, destroy_cmd_min};

	return (&f);
}