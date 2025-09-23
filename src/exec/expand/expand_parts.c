/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_parts.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 21:46:13 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/23 22:21:22 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// partごとに処理を進める関数
int	expand_one_part_buf(t_buf *b, t_arg_part *p, int is_head)
{
	t_buf		tmp;
	char		*u;
	const char	*base;

	if (!b || !p)
		return (-1);
	buf_init(&tmp);
	if (apply_tilde_if_head_buf(&tmp, p->text, p->quote, is_head) < 0)
		return (buf_free(&tmp), -1);
	if (tmp.data)
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
	if (buf_puts(b, u) < 0)
		return (free(u), -1);
	return (free(u), 0);
}

// 1partごとに処理する関数を束ねる関数
char	*parts_to_one_word(t_arg_part *parts)
{
	t_buf	b;
	int		is_head;

	buf_init(&b);
	is_head = 1;
	while (parts)
	{
		if (expand_one_part_buf(&b, parts, is_head) < 0)
			return (buf_free(&b), NULL);
		is_head = 0;
		parts = parts->next;
	}
	if (!b.data)
		return (ft_strdup(""));
	return (buf_take(&b));
}
