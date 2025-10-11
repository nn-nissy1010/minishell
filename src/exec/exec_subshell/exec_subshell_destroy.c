/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell_destroy.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 09:17:16 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/11 10:47:18 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	destroy_subshell_node(struct s_node *node)
{
	if (!node)
		return ;
	if (node->u_as.s_subshell.body && node->u_as.s_subshell.body->fn)
		node->u_as.s_subshell.body->fn->destroy(node->u_as.s_subshell.body);
	free(node);
}
