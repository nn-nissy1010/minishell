/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_buf_clean.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:16:43 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/23 22:21:00 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	buf_init(t_buf *b)
{
	if (!b)
		return ;
	b->data = NULL;
	b->cap = 0;
	b->len = 0;
}

void	buf_free(t_buf *b)
{
	if (!b)
		return ;
	if (b->data)
		free(b->data);
	b->data = NULL;
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