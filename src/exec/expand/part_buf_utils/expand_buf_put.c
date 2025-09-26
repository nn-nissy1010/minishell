/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_buf_put.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:49:57 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/25 12:54:12 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	buf_putc(t_buf *b, char c)
{
	if (!b)
		return (-1);
	if (buf_qreserve(b, 1) < 0)
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
	if (buf_qreserve(b, n) < 0)
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
