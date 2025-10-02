/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_child_direct.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 23:05:20 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/02 13:58:12 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_directory(const char *path)
{
	struct stat	st;

	if (!path)
		return (0);
	if (stat(path, &st) < 0)
		return (0);
	if (S_ISDIR(st.st_mode))
		return (1);
	return (0);
}

const char	*exec_errmsg(int err)
{
	if (err == ENOENT)
		return (": No such file or directory\n");
	if (err == EACCES)
		return (": Permission denied\n");
	if (err == EISDIR)
		return (": is a directory\n");
	if (err == ENOTDIR)
		return (": Not a directory\n");
	if (err == ENOEXEC)
		return (": Exec format error\n");
	return (": exec error\n");
}

void	fail_exec(const char *path, int err)
{
	err3("minishell: ", path, exec_errmsg(err));
	if (err == ENOENT)
		err = 127;
	else
		err = 126;
	exit(err);
}

void	free_env_array(char **envp)
{
	char	**p;

	if (!envp)
		return ;
	p = envp;
	while (*p)
		free(*p++);
	free(envp);
}

void	exec_direct(char **av)
{
	char	**envp;
	int		saved;

	if (!av || !av[0])
		exit(0);
	if (is_directory(av[0]))
	{
		err3("minishell: ", av[0], ": is a directory\n");
		exit(126);
	}
	envp = env_table_to_envp();
	if (!envp)
	{
		err3("minishell: ", "malloc", ": failed\n");
		exit(126);
	}
	execve(av[0], av, envp);
	saved = errno;
	free_env_array(envp);
	fail_exec(av[0], saved);
}
