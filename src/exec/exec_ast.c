/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:39:49 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/12 15:13:30 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc_error(int rc)
{
	if (rc == -2)
	{
		(void)write(STDOUT_FILENO, "\n", 1);
		set_exit_status(130);
		return (0);
	}
	set_exit_status(1);
	return (-1);
}

int	ast_execute_root(t_node *root, t_exec_ctx *ctx)
{
	int	rc;

	if (!root)
		return (0);
	rc = collect_heredocs_for_tree(root);
	if (rc != 0)
		return (handle_heredoc_error(rc));
	return (ast_exec(root, ctx));
}

int	ast_exec(t_node *node, t_exec_ctx *ctx)
{
	return (node->fn->exec(node, ctx));
}

void	ast_destroy(t_node *node)
{
	node->fn->destroy(node);
}
