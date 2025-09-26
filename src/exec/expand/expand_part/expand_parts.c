/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_parts.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 21:46:13 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/25 16:11:21 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static unsigned char	quote_to_mask(t_quote_type q)
{
	if (q == Q_SINGLE)
		return (Q_SINGLE);
	if (q == Q_DOUBLE)
		return (Q_DOUBLE);
	return (Q_NONE);
}

int	expand_one_part_buf(t_buf *b, t_arg_part *p, int is_head)
{
	t_buf			tmp;
	const char		*base;
	char			*u;
	unsigned char	q;

	if (!b || !p)
		return (-1);
	buf_init(&tmp);
	if (apply_tilde_if_head_buf(&tmp, p->text, p->quote, is_head) < 0)
		return (buf_free(&tmp), -1);
	if (tmp.data && tmp.data[0] != '\0')
		base = tmp.data;
	else
		base = "";
	if (p->has_param && p->quote != Q_SINGLE)
		u = expand_params_part(base);
	else
		u = ft_strdup(base);
	buf_free(&tmp);
	if (!u)
		return (-1);
	q = quote_to_mask(p->quote);
	if (buf_puts_q(b, u, q) < 0)
		return (free(u), -1);
	return (free(u), 0);
}

int	parts_to_buf(t_arg_part *parts, t_buf *out)
{
	int	is_head;

	is_head = 1;
	if (!out)
		return (-1);
	buf_init(out);
	while (parts)
	{
		if (expand_one_part_buf(out, parts, is_head) < 0)
			return (buf_free(out), -1);
		is_head = 0;
		parts = parts->next;
	}
	if (!out->data)
	{
		out->data = ft_strdup("");
		if (!out->data)
			return (-1);
	}
	return (0);
}
