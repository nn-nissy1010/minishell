/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:39:49 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/23 22:21:28 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ast_exec(t_node *node, t_exec_ctx *ctx)
{
	return (node->fn->exec(node, ctx));
}
void	ast_destroy(t_node *node)
{
	node->fn->destroy(node);
}
