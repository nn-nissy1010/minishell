/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_single.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 20:33:07 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/05 10:30:48 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_noop_cmd(const t_cmd *c)
{
	return (!c || !c->argv || !c->argv[0]);
}

static int	is_no_argv_but_has_redirs(const t_cmd *c)
{
	if (!c)
		return (0);
	return (c->argc == 0 && c->n_redirs > 0);
}

int	is_builtin_parent(const t_cmd *c)
{
	const char	*b;

	if (!c || !c->argv || !c->argv[0])
		return (0);
	b = c->argv[0];
	if (ft_strcmp(b, "cd") == 0)
		return (1);
	if (ft_strcmp(b, "export") == 0)
		return (1);
	if (ft_strcmp(b, "unset") == 0)
		return (1);
	if (ft_strcmp(b, "exit") == 0)
		return (1);
	return (0);
}

static int	in_sub_or_pipe_ctx(const t_exec_ctx *ctx)
{
	if (!ctx)
		return (0);
	if (ctx->xflag & (XF_PIPEIN | XF_PIPEOUT | XF_SUBSHELL))
		return (1);
	return (0);
}

int	exec_single(t_cmd *c, t_exec_ctx *ctx)
{
	pid_t	pid;

	if (!c)
		return (set_exit_status(2), -1);
	if (!in_sub_or_pipe_ctx(ctx) && is_builtin_parent(c))
		return (run_parent_builtin_flow(c));
	if (is_no_argv_but_has_redirs(c))
		return (redirect_only_flow(c));
	if (is_noop_cmd(c))
		return (set_exit_status(0), 0);
	pid = spawn_child(c);
	if (pid < 0)
	{
		perror("fork");
		return (set_exit_status(1), -1);
	}
	destroy_cmd_min_cmd(c);
	return (parent_finalize_simple(pid));
}
