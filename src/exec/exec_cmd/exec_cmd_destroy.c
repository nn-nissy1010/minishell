/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_destroy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:08:43 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/04 22:57:26 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_strarray_n(char **v, size_t n)
{
	size_t	i;

	if (!v)
		return ;
	i = 0;
	while (i < n)
	{
		if (v[i])
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
		free_strarray_n(cmd->argv, cmd->argc);
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

void	destroy_cmd_min(t_node *node)
{
	t_cmd	*c;

	if (!node)
		return ;
	c = &node->as.cmd;
	destroy_cmd_argv(c);
	destroy_cmd_tokens(c);
	destroy_cmd_redirs(c);
}
