/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 22:09:18 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/11 10:43:03 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		{
			(void)write(STDOUT_FILENO, "\n", 1);
			set_exit_status(130);
			return (end_with_error(c, 130));
		}
		return (set_exit_status(1), end_with_error(c, 1));
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
	if (prepare_cmd_for_exec(&node->u_as.cmd) != 0)
		return (-1);
	return (exec_single(&node->u_as.cmd, ctx));
}

t_func	*v_cmd(void)
{
	static t_func	f = {exec_cmd, destroy_cmd_min};

	return (&f);
}
