/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_destroy.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:57:40 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/06 21:12:18 by tkuwahat         ###   ########.fr       */
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
		left = p->as.bin.left;
		right = p->as.bin.right;
		p->as.bin.left = NULL;
		p->as.bin.right = NULL;
		destroy_branch_min(right);
		free(p);
		p = left;
	}
	destroy_branch_min(p);
}
