/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_destroy.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:57:40 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/05 09:21:39 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (right)
		{
			if (right->fn && right->fn->destroy)
				right->fn->destroy(right);
			else
				free(right);
		}
		free(p);
		p = left;
	}
	if (p)
	{
		if (p->fn && p->fn->destroy)
			p->fn->destroy(p);
		else
			free(p);
	}
}
