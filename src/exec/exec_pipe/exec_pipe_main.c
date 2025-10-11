/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:30:08 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/11 10:46:31 by nnishiya         ###   ########.fr       */
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

	if (!node || !node->u_as.s_bin.left || !node->u_as.s_bin.right)
		return (set_exit_status(2), -1);
	if (pipe(fds) < 0)
		return (perror("minishell: pipe"), set_exit_status(1), -1);
	if (call_pipe_children(node, parent_ctx, fds, &st_right) != 0)
		return (-1);
	set_exit_status(status_to_exitcode(st_right));
	return (0);
}

t_func	*v_pipe(void)
{
	static t_func	f = {exec_pipe, destroy_pipe_min};

	return (&f);
}
