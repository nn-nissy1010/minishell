/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell_main.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 09:17:16 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/11 10:48:21 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static pid_t	spawn_subshell_child(struct s_node *node,
		t_exec_ctx *parent_ctx)
{
	t_exec_ctx	ctx;
	pid_t		pid;

	if (!node || !node->u_as.s_subshell.body)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		set_exit_status(1);
		return (-1);
	}
	if (pid == 0)
	{
		ft_memset(&ctx, 0, sizeof(ctx));
		if (parent_ctx)
			ctx = *parent_ctx;
		ctx.xflag |= XF_SUBSHELL;
		set_child_signals_default();
		ast_exec(node->u_as.s_subshell.body, &ctx);
		exit(get_exit_status());
	}
	return (pid);
}

static int	wait_and_set_status(pid_t pid)
{
	int	st;

	while (1)
	{
		if (waitpid(pid, &st, 0) >= 0)
			break ;
		if (errno == EINTR)
			continue ;
		perror("waitpid");
		set_exit_status(1);
		return (-1);
	}
	if (WIFEXITED(st))
		set_exit_status(WEXITSTATUS(st));
	else if (WIFSIGNALED(st))
		set_exit_status(128 + WTERMSIG(st));
	else
		set_exit_status(1);
	return (0);
}

int	exec_subshell_node(struct s_node *node, t_exec_ctx *parent_ctx)
{
	pid_t	pid;

	if (!node || !node->u_as.s_subshell.body)
		return (set_exit_status(2), -1);
	pid = spawn_subshell_child(node, parent_ctx);
	if (pid < 0)
		return (-1);
	return (wait_and_set_status(pid));
}

t_func	*v_subshell(void)
{
	static t_func	v = {exec_subshell_node, destroy_subshell_node};

	return (&v);
}
