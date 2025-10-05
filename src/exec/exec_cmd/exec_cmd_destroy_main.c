/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_destroy_main.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:08:43 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/05 10:29:55 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	destroy_cmd_min(t_node *node)
{
	if (!node)
		return ;
	destroy_cmd_min_cmd(&node->as.cmd);
}
