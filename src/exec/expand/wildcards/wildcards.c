/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 23:06:27 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/06 16:23:56 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	push_literal_word(t_argbuf *dst, const char *s)
{
	char	*owned;

	if (!dst)
		return (-1);
	if (s != NULL)
		owned = ft_strdup(s);
	else
		owned = ft_strdup("");
	if (!owned)
		return (-1);
	if (argbuf_grow(dst, dst->n + 1) != 0)
	{
		free(owned);
		return (-1);
	}
	dst->v[dst->n++] = owned;
	return (0);
}

static int	expand_one_word(const char *word, t_argbuf *out)
{
	t_argbuf	m;
	int			rc;

	if (!should_glob_expand(word))
		return (push_literal_word(out, word));
	argbuf_init(&m);
	rc = expand_glob_pattern(word, &m);
	if (rc < 0)
		return (argbuf_free(&m), -1);
	if (m.n == 0)
	{
		rc = push_literal_word(out, word);
		argbuf_free(&m);
		return (rc);
	}
	sort_strptrs(m.v, m.n);
	return (argbuf_move_append(out, &m));
}

static int	gea_fail(t_cmd *c, t_argbuf *out, size_t i)
{
	while (i < (size_t)c->argc)
	{
		free(c->argv[i]);
		i++;
	}
	free(c->argv);
	c->argv = NULL;
	c->argc = 0;
	argbuf_free(out);
	return (-1);
}

static int	expand_argv_words(t_cmd *c, t_argbuf *out)
{
	size_t	i;

	i = 0;
	while (i < (size_t)c->argc)
	{
		if (expand_one_word(c->argv[i], out) != 0)
			return (gea_fail(c, out, i));
		free(c->argv[i]);
		i++;
	}
	return (0);
}

int	glob_expand_argv(t_cmd *c)
{
	t_argbuf	out;

	if (!c || c->argc <= 0 || !c->argv)
		return (0);
	argbuf_init(&out);
	if (expand_argv_words(c, &out) != 0)
		return (-1);
	free(c->argv);
	if (argbuf_terminate(&out) != 0)
	{
		argbuf_free(&out);
		c->argv = NULL;
		c->argc = 0;
		return (-1);
	}
	c->argv = out.v;
	c->argc = (int)out.n;
	return (0);
}
