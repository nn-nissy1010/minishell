/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_unquote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:43:33 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/20 16:06:39 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_unquoted_char(const char **p, t_buf *b, t_arg_part *proto)
{
	if (**p == '\\' && *(*p + 1))
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

t_arg_part	*scan_unquoted_part(const char **pp)
{
	const char	*p;
	t_buf		b;
	t_arg_part	proto;
	t_arg_part	*part;

	p = *pp;
	init_scan(&b, &proto, Q_NONE);
	while (*p && !ft_isspace((unsigned char)*p) && !ft_strchr("|&<>()'\"", *p))
		handle_unquoted_char(&p, &b, &proto);
	b.data = append_char(b.data, &b.cap, &b.len, '\0');
	*pp = p;
	part = new_part_copy(b.data, b.len - 1, &proto);
	free(b.data);
	return (part);
}
