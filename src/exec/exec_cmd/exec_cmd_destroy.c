/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_destroy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:08:43 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/05 22:21:30 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void free_strarray_nullterm(char **v)
{
    size_t i = 0;
    if (!v) return;
    while (v[i]) 
	{
        free(v[i]);
        i++;
    }
    free(v);
}

void	destroy_cmd_argv(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->argv)
	{
		free_strarray_nullterm(cmd->argv);
		cmd->argv = NULL;
		cmd->argc = 0;
	}
}

void	destroy_cmd_tokens(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->argv_tokens)
	{
		free(cmd->argv_tokens);
		cmd->argv_tokens = NULL;
		cmd->n_argv_tokens = 0;
	}
}

void	destroy_cmd_redirs(t_cmd *cmd)
{
	size_t	i;

	if (!cmd)
		return ;
	if (!cmd->redirs)
		return ;
	i = 0;
	while (i < cmd->n_redirs)
	{
		if (cmd->redirs[i].path)
			free(cmd->redirs[i].path);
		i++;
	}
	free(cmd->redirs);
	cmd->redirs = NULL;
	cmd->n_redirs = 0;
}

void	destroy_cmd_min_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	destroy_cmd_argv(cmd);
	destroy_cmd_tokens(cmd);
	destroy_cmd_redirs(cmd);
}

