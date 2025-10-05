/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_call_child.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:30:08 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/02 17:29:19 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void close_pipe_pair(int fds[2])
{
	safe_close(fds[0]);
	safe_close(fds[1]);
}

static int handle_left_spawn_fail(int fds[2], struct sigaction *oldint)
{
	close_pipe_pair(fds);
	parent_unmask_sigint(oldint);
	return (set_exit_status(1), -1);
}

static int handle_right_spawn_fail(int fds[2], pid_t lp, struct sigaction *oldint)
{
	close_pipe_pair(fds);
	(void)waitpid_retry(lp, NULL); /* 左だけ回収 */
	parent_unmask_sigint(oldint);
	return (set_exit_status(1), -1);
}

static void wait_both_and_take_right(pid_t lp, pid_t rp, int *st_right)
{
	(void)waitpid_retry(lp, NULL);       /* 左のコードは捨て */
	(void)waitpid_retry(rp, st_right);   /* 右端のコードを採用 */
}

int call_pipe_children(t_node *node, t_exec_ctx *parent_ctx, int fds[2], int *st_right)
{
	struct sigaction	oldint;
	pid_t				lp;
	pid_t				rp;

	parent_mask_sigint(&oldint);
	lp = spawn_pipe_child(node->as.bin.left, parent_ctx, fds, PIPE_LEFT);
	if (lp < 0)
		return handle_left_spawn_fail(fds, &oldint);

	rp = spawn_pipe_child(node->as.bin.right, parent_ctx, fds, PIPE_RIGHT);
	if (rp < 0)
		return handle_right_spawn_fail(fds, lp, &oldint);

	close_pipe_pair(fds);
	wait_both_and_take_right(lp, rp, st_right);

	parent_unmask_sigint(&oldint);
	return 0;
}