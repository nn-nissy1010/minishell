/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_buf_clean.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:16:43 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/16 16:05:24 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	buf_init(t_buf *b)
{
	if (!b)
		return ;
	b->data = NULL;
	b->qmask = NULL;
	b->cap = 0;
	b->len = 0;
}

void	buf_free(t_buf *b)
{
	if (!b)
		return ;
	if (b->data)
		free(b->data);
	if (b->qmask)
		free(b->qmask);
	b->data = NULL;
	b->qmask = NULL;
	b->cap = 0;
	b->len = 0;
}

void	buf_clear(t_buf *b)
{
	if (!b)
		return ;
	b->len = 0;
	if (b->data)
		b->data[0] = '\0';
}
