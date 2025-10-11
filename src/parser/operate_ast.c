/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operate_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:46:09 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/11 10:40:59 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	destroy_ast(t_node *n)
{
	if (!n)
		return ;
	if (n->type == ND_COMMAND)
		destroy_cmd_min_cmd(&n->u_as.cmd);
	else if (n->type == ND_PIPE || n->type == ND_AND_IF || n->type == ND_OR_IF)
	{
		destroy_ast(n->u_as.s_bin.left);
		destroy_ast(n->u_as.s_bin.right);
	}
	else if (n->type == ND_SUBSHELL)
		destroy_ast(n->u_as.s_subshell.body);
	free(n);
}

t_node	**ast_ref(void)
{
	static t_node	*ast = NULL;

	return (&ast);
}

void	set_ast(t_node *new_ast)
{
	*ast_ref() = new_ast;
}

t_node	*get_ast(void)
{
	return (*ast_ref());
}
