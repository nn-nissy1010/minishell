/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 00:33:34 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/29 18:19:05 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	argbuf_move_append(t_argbuf *dst, t_argbuf *src)
{
	if (argbuf_grow(dst, dst->n + src->n) != 0)
		return (-1);
	ft_memcpy(dst->v + dst->n, src->v, sizeof(char *) * src->n);
	dst->n += src->n;
	free(src->v);
	src->v = NULL;
	src->n = 0;
	src->cap = 0;
	return (0);
}

int	should_glob_expand(const char *s)
{
	int	has_star;

	if (!s)
		return (0);
	has_star = 0;
	while (*s)
	{
		if (*s == '*')
			has_star = 1;
		if (*s == '/')
			return (0);
		s++;
	}
	if (has_star)
		return (1);
	return (0);
}

int	pat_match(const char *p, const char *s)
{
	while (*p && *p != '*')
	{
		if (*s == '\0')
			return (0);
		if (*p != *s)
			return (0);
		p++;
		s++;
	}
	if (*p != '*')
		return (*s == '\0');
	while (*p == '*')
		p++;
	if (*p == '\0')
		return (1);
	while (*s)
	{
		if (pat_match(p, s))
			return (1);
		s++;
	}
	return (0);
}

static int	push_match_name(const char *name, t_argbuf *out, DIR *dir)
{
	char	*dup;

	dup = ft_strdup(name);
	if (!dup)
	{
		closedir(dir);
		return (-1);
	}
	if (argbuf_push(out, dup) != 0)
	{
		free(dup);
		closedir(dir);
		return (-1);
	}
	return (0);
}

int	collect_matches_in_cwd(const char *pattern, t_argbuf *out)
{
	DIR				*dir;
	struct dirent	*de;
	const char		*name;

	dir = opendir(".");
	if (!dir)
		return (-1);
	while (1)
	{
		de = readdir(dir);
		if (de == NULL)
			break ;
		name = de->d_name;
		if (!(pattern[0] == '.') && name[0] == '.')
			continue ;
		if (pat_match(pattern, name))
		{
			if (push_match_name(name, out, dir) != 0)
				return (-1);
		}
	}
	if (closedir(dir) != 0)
		return (-1);
	return (0);
}
