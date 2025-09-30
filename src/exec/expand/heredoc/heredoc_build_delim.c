/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_build_delim.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:05:11 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/29 17:13:34 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	hdoc_calc_need_and_quote(t_arg_part *part, int *quoted)
{
	size_t	need;
	size_t	len;

	need = 0;
	if (quoted)
		*quoted = 0;
	while (part)
	{
		if (part->text)
		{
			len = ft_strlen(part->text);
			need += len;
		}
		if (part->quote != Q_NONE && quoted)
			*quoted = 1;
		part = part->next;
	}
	return (need);
}

static void	hdoc_copy_parts_to(char *dst, t_arg_part *part)
{
	size_t	len;

	while (part)
	{
		if (part->text)
		{
			len = ft_strlen(part->text);
			ft_memcpy(dst, part->text, len);
			dst += len;
		}
		part = part->next;
	}
	*dst = '\0';
}

int	heredoc_build_delim_inplace(t_redir *r)
{
	size_t	need;

	if (!r || !r->word || !r->word->u.arg.parts)
		return (-1);
	need = hdoc_calc_need_and_quote(r->word->u.arg.parts, &r->quoted_heredoc);
	r->path = (char *)malloc(need + 1);
	if (!r->path)
		return (-1);
	hdoc_copy_parts_to(r->path, r->word->u.arg.parts);
	return (0);
}
