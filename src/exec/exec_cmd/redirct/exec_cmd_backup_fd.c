/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_apply_redir.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 21:44:47 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/01 20:24:07 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	rollback_and_invalidate(int *saved_in, int *saved_out)
{
	if (*saved_in != -1)
	{
		(void)dup2(*saved_in, STDIN_FILENO);
		close(*saved_in);
		*saved_in = -1;
	}
	if (*saved_out != -1)
	{
		(void)dup2(*saved_out, STDOUT_FILENO);
		close(*saved_out);
		*saved_out = -1;
	}
}
void	pre_backup_cleanup(int *saved_in, int *saved_out)
{
	if (*saved_in != -1)
	{
		close(*saved_in);
		*saved_in = -1;
	}
	if (*saved_out != -1)
	{
		close(*saved_out);
		*saved_out = -1;
	}
}
static int	backup_for_target(int tgt, int *saved_in, int *saved_out)
{
	if ((tgt == STDIN_FILENO) && (*saved_in == -1))
	{
		*saved_in = dup(STDIN_FILENO);
		if (*saved_in < 0)
			return (-1);
	}
	if ((tgt == STDOUT_FILENO) && (*saved_out == -1))
	{
		*saved_out = dup(STDOUT_FILENO);
		if (*saved_out < 0)
			return (-1);
	}
	return (0);
}
static int	target_fd(const t_redir *r)
{
	if (r->kind == TOK_REDIR_IN || r->kind == TOK_HEREDOC)
		return (STDIN_FILENO);
	if (r->fd >= 0)
		return (r->fd);
	return (STDOUT_FILENO);
}

int	pre_backup(const t_redir *r, size_t n, int *saved_in, int *saved_out)
{
	size_t	i;
	int		tgt;

	i = 0;
	while (i < n)
	{
		tgt = target_fd(&r[i]);
		if (backup_for_target(tgt, saved_in, saved_out) < 0)
		{
			pre_backup_cleanup(saved_in, saved_out);
			return (-1);
		}
		i++;
	}
	return (0);
}
