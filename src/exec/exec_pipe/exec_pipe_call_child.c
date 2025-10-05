/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_call_child.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:30:08 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/05 09:41:44 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_pipe_pair(int fds[2])
{
	safe_close(fds[0]);
	safe_close(fds[1]);
}

static int	handle_left_spawn_fail(int fds[2], struct sigaction *oldint)
{
	safe_close(fds[0]);
	safe_close(fds[1]);
	if (oldint)
		parent_unmask_sigint(oldint);
	set_exit_status(1);
	return (-1);
}

static int	handle_right_spawn_fail(int fds[2], pid_t lp,
		struct sigaction *oldint)
{
	safe_close(fds[0]);
	safe_close(fds[1]);
	(void)waitpid_retry(lp, NULL);
	if (oldint)
		parent_unmask_sigint(oldint);
	set_exit_status(1);
	return (-1);
}

static void	wait_both_and_take_right(pid_t lp, pid_t rp, int *st_right)
{
	int	st;

	st = 0;
	(void)waitpid_retry(lp, NULL);
	if (waitpid_retry(rp, &st) < 0)
		st = 0;
	if (st_right)
		*st_right = st;
}

int	call_pipe_children(t_node *node, t_exec_ctx *parent_ctx, int fds[2],
		int *st_right)
{
	struct sigaction	oldint;
	pid_t				lp;
	pid_t				rp;

	parent_mask_sigint(&oldint);
	lp = spawn_pipe_child(node->as.bin.left, parent_ctx, fds, PIPE_LEFT);
	if (lp < 0)
		return (handle_left_spawn_fail(fds, &oldint));
	rp = spawn_pipe_child(node->as.bin.right, parent_ctx, fds, PIPE_RIGHT);
	if (rp < 0)
		return (handle_right_spawn_fail(fds, lp, &oldint));
	close_pipe_pair(fds);
	wait_both_and_take_right(lp, rp, st_right);
	parent_unmask_sigint(&oldint);
	return (0);
}
