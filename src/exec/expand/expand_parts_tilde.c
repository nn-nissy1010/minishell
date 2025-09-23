/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_parts_tilde.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 21:51:31 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/23 22:21:19 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	apply_tilde_if_head_buf(t_buf *b, const char *s, t_quote_type q,
		int is_head)
{
	const char	*home;

	if (!b || !s)
		return (-1);
	if (!(is_head && q == Q_NONE && s[0] == '~'))
		return (buf_puts(b, s));
	if (s[1] && s[1] != '/')
		return (buf_puts(b, s));
	home = search_env_table("HOME");
	if (!home || home[0] == '\0')
		return (buf_puts(b, s));
	if (buf_puts(b, home) < 0)
		return (-1);
	if (s[1])
	{
		if (buf_puts(b, s + 1) < 0)
			return (-1);
	}
	return (0);
}
