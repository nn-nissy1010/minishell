/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_buf_clean.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:32:51 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/06 21:07:41 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	argbuf_init(t_argbuf *b)
{
	if (!b)
		return ;
	b->v = NULL;
	b->n = 0;
	b->cap = 0;
}

void	argbuf_free(t_argbuf *b)
{
	size_t	i;

	if (!b || !b->v)
		return ;
	i = 0;
	while (i < b->n)
	{
		free(b->v[i]);
		i++;
	}
	free(b->v);
	b->v = NULL;
	b->n = 0;
	b->cap = 0;
}
