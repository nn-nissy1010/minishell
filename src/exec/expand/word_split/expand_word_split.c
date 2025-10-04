/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word_split.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:59:20 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/04 22:47:30 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	argbuf_push_slice(t_argbuf *b, const char *p, size_t len)
{
	char	*s;

	if (!b)
		return (2);
	if (len > SIZE_MAX - 1)
		return (2);
	if (argbuf_grow(b, b->n + 1) != 0)
		return (2);
	s = (char *)malloc(len + 1);
	if (!s)
		return (2);
	if (len)
		ft_memcpy(s, p, len);
	s[len] = '\0';
	b->v[b->n] = s;
	b->n += 1;
	if (b->n < b->cap)
		b->v[b->n] = NULL;
	return (0);
}

int	split_fields_into_argbuf(const t_buf *b, t_argbuf *out)
{
	size_t	i;
	size_t	len;

	if (!b || !out)
		return (-1);
	i = skip_unquoted_ifs(b, 0);
	while (i < b->len)
	{
		len = field_span(b, i);
		if (len == 0)
		{
			i++;
			continue ;
		}
		if (argbuf_push_slice(out, b->data + i, len) != 0)
			return (-1);
		i += len;
		i = skip_unquoted_ifs(b, i);
	}
	return (0);
}
