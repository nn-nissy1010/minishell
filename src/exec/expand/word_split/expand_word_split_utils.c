/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word_split_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:58:46 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/25 12:58:56 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	arg_clear_items(t_arg *arg)
{
	size_t	i;

	if (!arg || !arg->items)
		return ;
	i = 0;
	while (i < arg->n_items)
	{
		free(arg->items[i]);
		i++;
	}
	free(arg->items);
	arg->items = NULL;
	arg->n_items = 0;
}

int	is_unquoted_ifs_at(const t_buf *b, size_t i)
{
	char	c;

	if (!b || !b->data || i >= b->len)
		return (0);
	c = b->data[i];
	return ((c == ' ' || c == '\t' || c == '\n') && (!b->qmask
			|| b->qmask[i] == Q_NONE));
}

size_t	skip_unquoted_ifs(const t_buf *b, size_t i)
{
	while (i < b->len && is_unquoted_ifs_at(b, i))
		i++;
	return (i);
}

size_t	field_span(const t_buf *b, size_t i)
{
	size_t	start;

	start = i;
	while (i < b->len && !is_unquoted_ifs_at(b, i))
		i++;
	return (i - start);
}

size_t	count_fields_from_buf(const t_buf *b)
{
	size_t	i;
	size_t	n;

	if (!b || !b->data || b->len == 0)
		return (0);
	i = 0;
	i = skip_unquoted_ifs(b, i);
	n = 0;
	while (i < b->len)
	{
		n++;
		i += field_span(b, i);
		i = skip_unquoted_ifs(b, i);
	}
	return (n);
}
