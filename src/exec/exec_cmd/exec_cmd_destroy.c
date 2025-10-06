/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_destroy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:08:43 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/06 15:55:20 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	destroy_cmd_argv(t_cmd *cmd)
{
	size_t	i;

	if (!cmd || !cmd->argv)
		return ;
	i = 0;
	while (cmd->argv[i])
		free(cmd->argv[i++]);
	free(cmd->argv);
	cmd->argv = NULL;
	cmd->argc = 0;
}

static void	destroy_arg_parts(t_arg_part *p)
{
	t_arg_part	*next;

	while (p)
	{
		next = p->next;
		if (p->text)
			free(p->text);
		free(p);
		p = next;
	}
}
void	destroy_token(t_token *t)
{
	size_t	k;

	if (!t)
		return ;
	if (t->type == TOK_ARG)
	{
		if (t->u.arg.raw)
			free(t->u.arg.raw);
		destroy_arg_parts(t->u.arg.parts);
		if (t->u.arg.items)
		{
			k = 0;
			while (k < t->u.arg.n_items)
			{
				free(t->u.arg.items[k]);
				k++;
			}
			free(t->u.arg.items);
		}
		t->u.arg.raw = NULL;
		t->u.arg.parts = NULL;
		t->u.arg.items = NULL;
		t->u.arg.n_items = 0;
	}
	free(t);
}

void	destroy_cmd_tokens(t_cmd *cmd)
{
	if (!cmd || !cmd->argv_tokens)
		return ;
	free(cmd->argv_tokens);
	cmd->argv_tokens = NULL;
	cmd->n_argv_tokens = 0;
}

static int	token_in_list(t_token *head, t_token *t)
{
	for (t_token *p = head; p; p = p->next)
		if (p == t)
			return (1);
	return (0);
}

void	destroy_cmd_redirs(t_cmd *cmd)
{
	size_t	i;
	t_token	*all;

	if (!cmd || !cmd->redirs)
		return ;
	all = get_tokens(); // この行のトークン連結リストの先頭
	i = 0;
	while (i < cmd->n_redirs)
	{
		if (cmd->redirs[i].hdoc_fd >= 0)
		{
			close(cmd->redirs[i].hdoc_fd);
			cmd->redirs[i].hdoc_fd = -1;
		}
		free(cmd->redirs[i].path);
		if (cmd->redirs[i].word)
		{
			if (!token_in_list(all, cmd->redirs[i].word))
				destroy_token(cmd->redirs[i].word);
			cmd->redirs[i].word = NULL;
		}
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
