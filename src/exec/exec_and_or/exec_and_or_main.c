/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_and_or_main.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 00:39:43 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/07 11:09:42 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_conditional(struct s_node *left, struct s_node *right,
		t_exec_ctx *ctx, int is_and)
{
	int	st;

	if (!left)
		return (set_exit_status(2), -1);
	ast_exec(left, ctx);
	st = get_exit_status();
	if ((is_and && st == 0) || (!is_and && st != 0))
	{
		if (right)
		{
			ast_exec(right, ctx);
			st = get_exit_status();
		}
	}
	set_exit_status(st);
	return (0);
}

static int	exec_and_if_node(struct s_node *node, t_exec_ctx *ctx)
{
	if (!node)
		return (set_exit_status(2), -1);
	return (exec_conditional(node->as.bin.left, node->as.bin.right, ctx, 1));
}

static int	exec_or_if_node(struct s_node *node, t_exec_ctx *ctx)
{
	if (!node)
		return (set_exit_status(2), -1);
	return (exec_conditional(node->as.bin.left, node->as.bin.right, ctx, 0));
}

t_func	*v_and_if(void)
{
	static t_func	v = {exec_and_if_node, destroy_bin_node};

	return (&v);
}

t_func	*v_or_if(void)
{
	static t_func	v = {exec_or_if_node, destroy_bin_node};

	return (&v);
}
