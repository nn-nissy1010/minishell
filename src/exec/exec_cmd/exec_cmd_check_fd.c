/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_check_fd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 22:14:36 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/06 22:21:00 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	dup2_checked(int oldfd, int newfd)
{
	if (oldfd == newfd)
		return (0);
	if (dup2(oldfd, newfd) < 0)
		return (-1);
	return (0);
}

void	restore_stdio(int saved_in, int saved_out)
{
	if (saved_in >= 0)
	{
		(void)dup2_checked(saved_in, STDIN_FILENO);
		close(saved_in);
	}
	if (saved_out >= 0)
	{
		(void)dup2_checked(saved_out, STDOUT_FILENO);
		close(saved_out);
	}
}

void	close_heredoc_fds(t_cmd *c)
{
	size_t	i;

	if (!c || !c->redirs)
		return ;
	i = 0;
	while (i < c->n_redirs)
	{
		if (c->redirs[i].kind == TOK_HEREDOC && c->redirs[i].hdoc_fd >= 0)
		{
			close(c->redirs[i].hdoc_fd);
			c->redirs[i].hdoc_fd = -1;
		}
		i++;
	}
}

int	end_with_error(t_cmd *c, int status_set)
{
	if (status_set >= 0)
		set_exit_status(status_set);
	close_heredoc_fds(c);
	return (-1);
}
