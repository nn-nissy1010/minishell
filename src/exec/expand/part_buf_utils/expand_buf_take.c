/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_buf_take.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:17:56 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/16 14:54:32 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*buf_take(t_buf *b)
{
	char	*p;

	if (!b || !b->data)
		return (NULL);
	p = b->data;
	b->data = NULL;
	b->cap = 0;
	b->len = 0;
	return (p);
}

int	buf_take_pair(t_buf *b, char **out_data, unsigned char **out_qmask)
{
	if (!b || !out_data || !out_qmask)
		return (-1);
	*out_data = b->data;
	*out_qmask = b->qmask;
	b->data = NULL;
	b->qmask = NULL;
	b->cap = 0;
	b->len = 0;
	return (0);
}
