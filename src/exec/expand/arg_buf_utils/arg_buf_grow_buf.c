/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_buf_grow_buf.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:37:46 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/04 23:16:19 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	calc_newcap(size_t cur_cap, size_t need_count, size_t *out_cap)
{
	size_t	cap;

	if (!out_cap)
		return (2);
	if (need_count > SIZE_MAX / sizeof(char *))
		return (2);
	cap = cur_cap;
	if (cap == 0)
		cap = 8;
	while (cap < need_count)
	{
		if (cap > SIZE_MAX / 2)
		{
			cap = need_count;
			break ;
		}
		cap *= 2;
	}
	if (cap > SIZE_MAX / sizeof(char *))
		return (2);
	*out_cap = cap;
	return (0);
}

static int	resize_argbuf_to(t_argbuf *b, size_t newcap)
{
	char	**nv;

	if (!b)
		return (2);
	nv = (char **)malloc(sizeof(char *) * newcap);
	if (!nv)
		return (2);
	if (b->v && b->n > 0)
		ft_memcpy(nv, b->v, sizeof(char *) * b->n);
	if (b->n < newcap)
		nv[b->n] = NULL;
	free(b->v);
	b->v = nv;
	b->cap = newcap;
	return (0);
}

int	argbuf_grow(t_argbuf *b, size_t need_count)
{
	size_t	newcap;

	if (!b)
		return (2);
	if (b->cap >= need_count)
		return (0);
	if (calc_newcap(b->cap, need_count, &newcap) != 0)
		return (2);
	return (resize_argbuf_to(b, newcap));
}
