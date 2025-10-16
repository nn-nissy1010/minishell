/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_bi_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 01:44:26 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/17 00:36:52 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static void	putline_fd(const char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
	write(fd, "\n", 1);
}

static int	env_invalid_arg(char **av)
{
	if (av && av[1])
	{
		write(STDERR_FILENO, "minishell: env: ", 16);
		write(STDERR_FILENO, av[1], ft_strlen(av[1]));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		return (127);
	}
	return (0);
}

int	bi_env(char **av)
{
	char	**envp;
	char	**p;
	char	*eq;
	int		rc;

	rc = env_invalid_arg(av);
	if (rc)
		return (rc);
	envp = env_table_to_envp();
	if (!envp)
	{
		write(STDERR_FILENO, "minishell: env: malloc failed\n", 30);
		return (1);
	}
	p = envp;
	while (*p)
	{
		eq = ft_strchr(*p, '=');
		if (eq)
			putline_fd(*p, STDOUT_FILENO);
		p++;
	}
	free_env_array(envp);
	return (0);
}
