/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:30:08 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/02 17:49:13 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	parent_unmask_sigint(const struct sigaction *old)
{
	x_sigaction(SIGINT, (struct sigaction *)old);
}

int	exec_pipe(t_node *node, t_exec_ctx *parent_ctx)
{
	int	fds[2];
	int	st_right;

	if (!node || !node->as.bin.left || !node->as.bin.right)
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
