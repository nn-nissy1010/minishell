/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_parts.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 21:46:13 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/17 01:05:12 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned char	quote_to_mask(t_quote_type q)
{
	if (q == Q_SINGLE)
		return (Q_SINGLE);
	if (q == Q_DOUBLE)
		return (Q_DOUBLE);
	return (Q_NONE);
}

unsigned char	quote_to_mask_and_mask_globs(char *s, t_quote_type q)
{
	if (q != Q_NONE && s)
	{
		while (*s)
		{
			if (*s == '*')
				*s = '\a';
			s++;
		}
	}
	return (quote_to_mask(q));
}

char	*expand_params(int has_param, t_quote_type quote, const char *base)
{
	char	*u;

	if (has_param && quote != Q_SINGLE)
		u = expand_params_part(base);
	else
	{
		if (base[0] == '\0')
			u = ft_strdup("");
		else
			u = ft_strdup(base);
	}
	return (u);
}

int	expand_one_part_buf(t_buf *b, t_arg_part *p, int is_head)
{
	t_buf			tmp;
	const char		*base;
	char			*u;
	unsigned char	q;

	u = 0;
	base = 0;
	if (!b || !p)
		return (-1);
	buf_init(&tmp);
	if (apply_tilde_if_head_buf(&tmp, p->text, p->quote, is_head) < 0)
		return (buf_free(&tmp), -1);
	if (tmp.data && tmp.data[0] != '\0')
		base = tmp.data;
	else
		base = "";
	u = expand_params(p->has_param, p->quote, base);
	buf_free(&tmp);
	if (!u)
		return (-1);
	q = quote_to_mask_and_mask_globs(u, p->quote);
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
		return (-1);
	}
	return (0);
}
