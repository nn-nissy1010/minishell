/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_buf_take.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:17:56 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/29 14:17:51 by tkuwahat         ###   ########.fr       */
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
