/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:30:08 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/12 13:05:20 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parent_unmask_sigint(struct sigaction *old)
{
	(void)sigaction(SIGINT, old, NULL);
}

int	exec_pipe(t_node *node, t_exec_ctx *parent_ctx)
{
	int	fds[2];
	int	st_right;
	int rc;

	if (!node || !node->u_as.s_bin.left || !node->u_as.s_bin.right)
		return (set_exit_status(2), -1);
	if (pipe(fds) < 0)
		return (perror("minishell: pipe"), set_exit_status(1), -1);
	rc = call_pipe_children(node, parent_ctx, fds, &st_right);
	if (rc < 0)
		return (-1);
	if (rc == 1) 
		set_exit_status(130);
	else
		set_exit_status(status_to_exitcode(st_right));
	return (0);
}

t_func	*v_pipe(void)
{
	static t_func	f = {exec_pipe, destroy_pipe_min};

	return (&f);
}
