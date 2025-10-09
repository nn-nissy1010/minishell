/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_and_or_destroy.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 00:39:43 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/07 01:02:13 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	destroy_bin_node(struct s_node *node)
{
	if (!node)
		return ;
	if (node->as.bin.left)
		node->as.bin.left->fn->destroy(node->as.bin.left);
	if (node->as.bin.right)
		node->as.bin.right->fn->destroy(node->as.bin.right);
	free(node);
}
