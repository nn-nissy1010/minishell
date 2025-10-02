/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_flow_child.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:30:08 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/02 17:19:40 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_prepare_ctx(t_exec_ctx *dst, const t_exec_ctx *parent,
		int is_left)
{
	*dst = *parent;
	if (is_left)
		dst->xflag |= XF_PIPEOUT;
	else
		dst->xflag |= XF_PIPEIN;
}


static void	child_setup_left_io(int fds[2])
{
	safe_close(fds[0]);
	if (dup2(fds[1], STDOUT_FILENO) < 0)
	{
		perror("dup2");
		exit(1);
	}
	safe_close(fds[1]);
}


static void	child_setup_right_io(int fds[2])
{
	safe_close(fds[1]);
	if (dup2(fds[0], STDIN_FILENO) < 0)
	{
		perror("dup2");
		exit(1);
	}
	safe_close(fds[0]);
}


static void	child_exec_and_exit(t_node *n, t_exec_ctx *c)
{
	(void)ast_exec(n, c);
	exit(get_exit_status()); 
}

pid_t	spawn_pipe_child(t_node *n, t_exec_ctx *parent_ctx, int fds[2], int is_left)
{
	pid_t		pid;
	t_exec_ctx	c;

	pid = fork();
	if (pid == 0)
	{
		reset_child_signals();
		child_prepare_ctx(&c, parent_ctx, is_left);
		if (is_left)
			child_setup_left_io(fds);
		else
			child_setup_right_io(fds);
		child_exec_and_exit(n, &c);
	}
	if (pid < 0)
		perror("minishell: fork");
	return (pid);
}



