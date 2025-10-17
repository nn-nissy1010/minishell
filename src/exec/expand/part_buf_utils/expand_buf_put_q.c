/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_buf_put_q.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:54:33 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/17 00:38:13 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	buf_putc_q(t_buf *b, char c, unsigned char q)
{
	if (!b)
		return (-1);
	if (buf_qreserve(b, 1) < 0)
		return (-1);
	b->data[b->len] = c;
	b->qmask[b->len] = q;
	b->len++;
	b->data[b->len] = '\0';
	return (0);
}

int	buf_putn_q(t_buf *b, const char *p, size_t n, unsigned char q)
{
	if (p[0] == '\0')
	{
		buf_qreserve(b, 1);
		b->qmask = 0;
		b->len = 1;
		b->data[0] = '\0';
		return (0);
	}
	if (!b || (!p && n > 0))
		return (-1);
	if (n == 0)
		return (0);
	if (buf_qreserve(b, n) < 0)
		return (-1);
	ft_memcpy(b->data + b->len, p, n);
	ft_memset(b->qmask + b->len, q, n);
	b->len += n;
	b->data[b->len] = '\0';
	return (0);
}

int	buf_puts_q(t_buf *b, const char *s, unsigned char q)
{
	size_t	n;

	if (!b)
		return (-1);
	if (!s)
		return (0);
	n = ft_strlen(s);
	return (buf_putn_q(b, s, n, q));
}
