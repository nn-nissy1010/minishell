/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_dir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:46:18 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/02 01:28:46 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	needs_dot_prefix(const char *s)
{
	if (!s)
		return (0);
	if (s[0] == '.' && s[1] == '/')
		return (0);
	return (1);
}

static int	prepend_dot_slash(char **ps)
{
	char	*s;
	char	*p;
	size_t	len;

	if (!ps || !*ps)
		return (0);
	s = *ps;
	len = ft_strlen(s);
	p = (char *)malloc(len + 3);
	if (!p)
		return (-1);
	p[0] = '.';
	p[1] = '/';
	ft_memcpy(p + 2, s, len + 1);
	free(s);
	*ps = p;
	return (0);
}

int	add_dot_prefix(t_argbuf *out)
{
	size_t	i;

	if (!out || !out->v)
		return (0);
	i = 0;
	while (i < out->n)
	{
		if (needs_dot_prefix(out->v[i]))
		{
			if (prepend_dot_slash(&out->v[i]) != 0)
				return (-1);
		}
		i++;
	}
	return (0);
}

static int	expand_glob_only_cwd(const char *name_pat, int add_dotprefix,
		t_argbuf *out)
{
	int		rc;
	char	star[2];

	if (!name_pat || name_pat[0] == '\0')
	{
		star[0] = '*';
		star[1] = '\0';
		name_pat = star;
	}
	rc = collect_matches_in_cwd(name_pat, out);
	if (rc != 0)
		return (-1);
	if (add_dotprefix && out && out->n > 0)
		return (add_dot_prefix(out));
	return (0);
}

int	expand_glob_pattern(const char *pattern, t_argbuf *out)
{
	int			add_dotprefix;
	const char	*slash;

	add_dotprefix = 0;
	slash = NULL;
	if (ft_strncmp(pattern, "./", 2) == 0)
		add_dotprefix = 1;
	slash = ft_strchr(pattern, '/');
	if (slash == NULL)
		return (collect_matches_in_cwd(pattern, out));
	if (add_dotprefix && ft_strchr(slash + 1, '/') == NULL)
		return (expand_glob_only_cwd(slash + 1, add_dotprefix, out));
	return (0);
}
