/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_destroy.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:57:40 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/11 10:45:17 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	destroy_branch_min(t_node *n)
{
	if (!n)
		return ;
	if (n->fn && n->fn->destroy)
		n->fn->destroy(n);
	else
		free(n);
}

void	destroy_pipe_min(t_node *node)
{
	t_node	*p;
	t_node	*left;
	t_node	*right;

	p = node;
	while (p && p->type == ND_PIPE)
	{
		left = p->u_as.s_bin.left;
		right = p->u_as.s_bin.right;
		p->u_as.s_bin.left = NULL;
		p->u_as.s_bin.right = NULL;
		destroy_branch_min(right);
		free(p);
		p = left;
	}
	destroy_branch_min(p);
}
