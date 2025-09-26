/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_buf_emit.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:13:23 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/25 12:53:39 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	emit_int_itoa(t_buf *b, int v)
{
	char	*s;
	int		r;

	s = ft_itoa(v);
	if (!s)
		return (-1);
	r = buf_puts(b, s);
	free(s);
	return (r);
}

int	emit_status(t_buf *b)
{
	return (emit_int_itoa(b, get_exit_status()));
}

int	emit_env(t_buf *b, const char *name)
{
	const char	*v;

	v = search_env_table(name);
	if (!v)
		v = "";
	return (buf_puts(b, v));
}
