/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 00:33:34 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/30 20:49:31 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	push_match_name(const char *name, t_argbuf *out)
{
	char	*dup;

	dup = ft_strdup(name);
	if (!dup)
		return (-1);
	if (argbuf_push(out, dup) != 0)
	{
		free(dup);
		return (-1);
	}
	return (0);
}

static int	should_skip_hidden(const char *pattern, const char *name)
{
	int	pattern_allows_hidden;
	int	is_dot_or_dotdot;

	if (!name || name[0] != '.')
		return (0);
	pattern_allows_hidden = (pattern && pattern[0] == '.');
	if (!pattern_allows_hidden)
		return (1);
	is_dot_or_dotdot = (name[1] == '\0' || (name[1] == '.' && name[2] == '\0'));
	if (is_dot_or_dotdot && ft_strcmp(pattern, name) != 0)
		return (1);
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
		if (should_skip_hidden(pattern, name))
			continue ;
		if (pat_match(pattern, name))
		{
			if (push_match_name(name, out) != 0)
				return ((void)closedir(dir), -1);
		}
	}
	if (closedir(dir) != 0)
		return (-1);
	return (0);
}
