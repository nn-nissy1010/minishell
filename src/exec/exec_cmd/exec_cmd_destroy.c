/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_destroy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:08:43 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/06 23:58:51 by tkuwahat         ###   ########.fr       */
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

int	token_in_list(t_token *head, t_token *t)
{
	while (head)
	{
		if (head == t)
			return (1);
		head = head->next;
	}
	return (0);
}

void    destroy_cmd_redirs(t_cmd *cmd)
{
    t_redir *r;
    t_redir *end;

    if (!cmd || !cmd->redirs)
        return;
    r = cmd->redirs;
    end = r + cmd->n_redirs;
    while (r < end)
    {
        if (r->hdoc_fd >= 0)
        {
            close(r->hdoc_fd);
            r->hdoc_fd = -1;
        }
        free(r->path);      
        r->path = NULL;
        r->word = NULL;     
        r++;
    }
    free(cmd->redirs);
    cmd->redirs = NULL;
    cmd->n_redirs = 0;
}

