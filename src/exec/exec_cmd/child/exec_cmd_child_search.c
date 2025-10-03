/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_child_search.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 23:28:56 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/02 22:33:24 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*dup_path_or_exit(const char *cmd)
{
	const char	*path;
	char		*dup;

	path = search_env_table("PATH");
	if (!path || *path == '\0')
	{
		err3("minishell: ", cmd, ": command not found\n");
		exit(127);
	}
	dup = ft_strdup(path);
	if (!dup)
	{
		err3("minishell: ", "malloc", ": failed\n");
		exit(126);
	}
	return (dup);
}

void	finalize_path_search(const char *cmd, const t_probe_flags *f)
{
	if (f->found_dir)
	{
		err3("minishell: ", cmd, ": is a directory\n");
		exit(126);
	}
	if (f->found_noexec)
	{
		err3("minishell: ", cmd, ": Permission denied\n");
		exit(126);
	}
	err3("minishell: ", cmd, ": command not found\n");
	exit(127);
}

static void	probe_path_elem(const char *elem, char **av, char **envp,
		t_probe_flags *f)
{
	if (elem && *elem == '\0')
		probe_dir_entry(".", av, envp, f);
	else
		probe_dir_entry(elem, av, envp, f);
}

static void	scan_path_and_try(char *dup, char **av, char **envp,
		t_probe_flags *f)
{
	char	*cur;
	char	*next;

	cur = dup;
	while (1)
	{
		next = ft_strchr(cur, ':');
		if (next)
			*next = '\0';
		probe_path_elem(cur, av, envp, f);
		if (!next)
			break ;
		cur = next + 1;
	}
}

void	search_and_exec(char **av)
{
	char			*dup;
	char			*orig;
	t_probe_flags	f;
	char			**envp;

	if (!av || !av[0])
		exit(0);
	envp = env_table_to_envp();
	if (!envp)
	{
		err3("minishell: ", "malloc", ": failed\n");
		exit(126);
	}
	ft_memset(&f, 0, sizeof(f));
	dup = dup_path_or_exit(av[0]);
	orig = dup;
	f.found_dir = 0;
	f.found_noexec = 0;
	scan_path_and_try(dup, av, envp, &f);
	free_env_array(envp);
	free(orig);
	finalize_path_search(av[0], &f);
}
