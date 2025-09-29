/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_buf_put.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:49:57 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/29 14:19:23 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	buf_realloc_to(t_buf *b, size_t need)
{
	size_t	ncap;
	char	*np;

	ncap = b->cap;
	if (ncap == 0)
		ncap = 64;
	while (ncap < need)
	{
		if (ncap > ((size_t)1 / 2))
			return (-1);
		ncap *= 2;
	}
	np = (char *)malloc(ncap);
	if (np == NULL)
		return (-1);
	if (b->len > 0 && b->data != NULL)
		ft_memcpy(np, b->data, b->len);
	if (b->data != NULL)
		free(b->data);
	b->data = np;
	b->cap = ncap;
	if (b->len == 0)
		b->data[0] = '\0';
	return (0);
}

int	buf_reserve(t_buf *b, size_t add)
{
	size_t	need;

	if (b == NULL)
		return (-1);
	if (add == 0)
		return (0);
	if (b->cap >= b->len + add + 1)
		return (0);
	need = b->len + add + 1;
	return (buf_realloc_to(b, need));
}

int	buf_putc(t_buf *b, char c)
{
	if (!b)
		return (-1);
	if (buf_reserve(b, 1) < 0)
		return (-1);
	b->data[b->len++] = c;
	b->data[b->len] = '\0';
	return (0);
}

int	buf_putn(t_buf *b, const void *p, size_t n)
{
	if (!b || (!p && n > 0))
		return (-1);
	if (n == 0)
		return (0);
	if (buf_reserve(b, n) < 0)
		return (-1);
	ft_memcpy(b->data + b->len, p, n);
	b->len += n;
	b->data[b->len] = '\0';
	return (0);
}

int	buf_puts(t_buf *b, const char *s)
{
	size_t	n;

	if (!b)
		return (-1);
	if (!s)
		return (0);
	n = ft_strlen(s);
	return (buf_putn(b, s, n));
}
