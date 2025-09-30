/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_buf_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:37:46 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/30 20:24:36 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	argbuf_grow(t_argbuf *b, size_t newcap)
{
	char	**nv;
	size_t	bytes;

	if (!b)
		return (2);
	if (newcap <= b->cap)
		return (0);
	nv = (char **)malloc(sizeof(char *) * newcap);
	if (!nv)
		return (2);
	if (b->v && b->n > 0)
	{
		bytes = sizeof(char *) * b->n;
		ft_memcpy(nv, b->v, bytes);
	}
	free(b->v);
	b->v = nv;
	b->cap = newcap;
	return (0);
}

int	argbuf_push(t_argbuf *b, const char *s)
{
	char	*dup;
	size_t	newcap;

	if (!b)
		return (2);
	if (b->n + 1 >= b->cap)
	{
		if (b->cap == 0)
			newcap = 8;
		else
			newcap = b->cap * 2;
		if (argbuf_grow(b, newcap) != 0)
			return (2);
	}
	if (s)
		dup = ft_strdup(s);
	else
		dup = ft_strdup("");
	if (!dup)
		return (2);
	b->v[b->n++] = dup;
	return (0);
}

int	argbuf_terminate(t_argbuf *b)
{
	size_t	need;

	if (!b)
		return (2);
	need = b->n + 1;
	if (b->cap < need)
	{
		if (argbuf_grow(b, need) != 0)
			return (2);
	}
	b->v[b->n] = NULL;
	return (0);
}

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
