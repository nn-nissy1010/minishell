/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_i_o.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 22:14:36 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/01 22:02:00 by tkuwahat         ###   ########.fr       */
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
