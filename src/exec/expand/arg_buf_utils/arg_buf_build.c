/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_buf_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:37:46 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/02 22:58:06 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int argbuf_grow(t_argbuf *b, size_t need_count)
{
    size_t  newcap;
    char  **nv;

    if (!b)
        return (2);
    if (b->cap >= need_count)
        return (0);
    if (need_count > SIZE_MAX / sizeof(char *))
        return (2);
    if (b->cap == 0)
        newcap = 8;
    else
        newcap = b->cap;
    while (newcap < need_count)
    {
        if (newcap > SIZE_MAX / 2)
        {
            newcap = need_count;
            break;
        }
        newcap *= 2;
    }
    if (newcap > SIZE_MAX / sizeof(char *))
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


int argbuf_push(t_argbuf *b, const char *s)
{
    char *dup;

    if (!b)
        return (2);
    if (argbuf_grow(b, b->n + 1) != 0)
        return (2);

    if (s)
        dup = ft_strdup(s);
    else
        dup = ft_strdup("");

    if (!dup)
        return (2);

    b->v[b->n++] = dup;
    b->v[b->n] = NULL; 
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
