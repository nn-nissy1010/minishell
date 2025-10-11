/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_quote.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:42:00 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/11 09:35:35 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_quote_type	get_quote_type(char q)
{
	if (q == '\'')
		return (Q_SINGLE);
	return (Q_DOUBLE);
}

static void	handle_char(const char **p, t_buf *b, t_arg_part *proto)
{
	if (proto->quote == Q_DOUBLE && **p == '\\' && (*(*p + 1) == '"' || *(*p
				+ 1) == '\\' || *(*p + 1) == '$'))
	{
		(*p)++;
		b->data = append_char(b->data, &b->cap, &b->len, *(*p)++);
	}
	else
	{
		if (**p == '$')
			proto->has_param = 1;
		if (**p == '*')
			proto->has_unq_glob = 1;
		b->data = append_char(b->data, &b->cap, &b->len, *(*p)++);
	}
}

t_arg_part	*scan_quoted_part(const char **pp)
{
	const char	*p;
	char		q;
	t_arg_part	proto;
	t_buf		b;
	t_arg_part	*part;

	p = *pp;
	q = *p;
	p++;
	init_scan(&b, &proto, get_quote_type(q));
	while (*p && *p != q)
		handle_char(&p, &b, &proto);
	if (!*p)
	{
		write(STDERR_FILENO, "minishell: unmatched quote\n", 27);
		return (free(b.data), NULL);
	}
	p++;
	b.data = append_char(b.data, &b.cap, &b.len, '\0');
	*pp = p;
	part = new_part_copy(b.data, b.len, &proto);
	free(b.data);
	return (part);
}
