/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_and_or_destroy.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 00:39:43 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/11 10:42:36 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	destroy_bin_node(struct s_node *node)
{
	if (!node)
		return ;
	if (node->u_as.s_bin.left)
		node->u_as.s_bin.left->fn->destroy(node->u_as.s_bin.left);
	if (node->u_as.s_bin.right)
		node->u_as.s_bin.right->fn->destroy(node->u_as.s_bin.right);
	free(node);
}
