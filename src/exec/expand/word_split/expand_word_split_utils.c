/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word_split_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:58:46 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/03 01:02:43 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

