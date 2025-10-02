/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_child_search_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 19:17:42 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/02 11:24:01 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	buf_cat(char *buf, size_t *pn, size_t cap, const char *s)
{
	size_t	avail;
	size_t	w;

	if (*pn >= cap)
		return ;
	avail = cap - *pn;
	if (!s)
		s = "";
	w = ft_strlcpy(buf + *pn, s, avail);
	if (w < avail)
		*pn += w;
	else
		*pn += avail - 1;
}

static char	*path_join(const char *dir, const char *cmd)
{
	size_t	dl;
	size_t	cl;
	size_t	need_slash;
	char	*p;

	dl = ft_strlen(dir);
	cl = ft_strlen(cmd);
	if (dl > 0 && dir[dl - 1] != '/')
		need_slash = 1;
	else
		need_slash = 0;
	p = (char *)malloc(dl + need_slash + cl + 1);
	if (!p)
		return (NULL);
	ft_memcpy(p, dir, dl);
	if (need_slash)
	{
		p[dl] = '/';
		dl++;
	}
	ft_memcpy(p + dl, cmd, cl);
	p[dl + cl] = '\0';
	return (p);
}

void	probe_dir_entry(const char *dir, char **av, char **envp,
		t_probe_flags *f)
{
	char	*full;
	int		saved;

	full = path_join(dir, av[0]);
	if (!full)
		return ;
	if (access(full, F_OK) != 0)
	{
		free(full);
		return ;
	}
	if (is_directory(full))
	{
		f->found_dir = 1;
		free(full);
		return ;
	}
	execve(full, av, envp);
	saved = errno;
	free(full);
	if (saved == ENOEXEC)
		exit_enoexec(av[0]);
	if (saved == EACCES || saved == EPERM)
		f->found_noexec = 1;
}
