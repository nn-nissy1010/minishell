/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_bi_pwd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 01:44:26 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/02 00:36:56 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <stdlib.h> /* free */
#include <string.h> /* strerror */
#include <unistd.h>

static void	putline_fd(const char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
	write(fd, "\n", 1);
}

int	bi_pwd(char **av)
{
	char		*cwd;
	const char	*pwd;
	const char	*msg;

	(void)av;
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		putline_fd(cwd, STDOUT_FILENO);
		free(cwd);
		return (0);
	}
	pwd = search_env_table("PWD");
	if (pwd && *pwd)
	{
		putline_fd(pwd, STDOUT_FILENO);
		return (0);
	}
	write(STDERR_FILENO, "minishell: pwd: getcwd: ", 24);
	msg = strerror(errno);
	if (msg)
		write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	return (1);
}
