/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_destroy_main.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:08:43 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/09 13:36:19 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	destroy_cmd_tokens(t_cmd *cmd)
{
	if (!cmd || !cmd->argv_tokens)
		return ;
	free(cmd->argv_tokens);
	cmd->argv_tokens = NULL;
	cmd->n_argv_tokens = 0;
}

void	destroy_cmd_min_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	destroy_cmd_argv(cmd);
	destroy_cmd_tokens(cmd);
	destroy_cmd_redirs(cmd);
}

void	destroy_cmd_min(t_node *node)
{
	if (!node)
		return ;
	destroy_cmd_min_cmd(&node->as.cmd);
}
