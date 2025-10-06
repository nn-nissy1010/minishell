/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_child_external.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 22:56:31 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/06 20:03:20 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_slash(const char *s)
{
	if (!s)
		return (0);
	if (ft_strchr(s, '/'))
		return (1);
	return (0);
}

void	close_extra_fds(void)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
	{
		close(fd);
		fd++;
	}
}

void	run_external_in_child(t_cmd *c)
{
	if (!c || !c->argv || !c->argv[0])
		exit(0);
	if (has_slash(c->argv[0]))
	{
		if (is_directory(c->argv[0]))
		{
			err3("minishell: ", c->argv[0], ": is a directory\n");
			exit(126);
		}
		exec_direct(c->argv);
	}
	else
		search_and_exec(c->argv);
	exit(127);
}
