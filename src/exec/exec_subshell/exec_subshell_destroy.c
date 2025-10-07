/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell_destroy.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 09:17:16 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/07 09:41:12 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	destroy_subshell_node(struct s_node *node)
{
	if (!node)
		return ;
	if (node->as.subshell.body && node->as.subshell.body->fn)
		node->as.subshell.body->fn->destroy(node->as.subshell.body);
	free(node);
}
