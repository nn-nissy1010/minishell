/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:57:45 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/13 16:01:19 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_more	check_unclosed_quote(const char *s)
{
	int		q;
	size_t	i;

	q = 0;
	i = 0;
	while (s[i])
	{
		char c = s[i];
		if (q == 0)
			handle_no_quote(c, &q, &i, s);
		else if (q == '\'')
			handle_single_quote(c, &q);
		else
			handle_double_quote(c, &q, i, s);
		i++;
	}
	if (q == '\'')
		return (MORE_QUOTE_S);
	if (q == '"')
		return (MORE_QUOTE_D);
	return (MORE_NONE);
}

static t_more	check_line_end_operator(const char *s)
{
	int	i;

	if (!s || !*s)
		return (MORE_NONE);
	i = ft_strlen(s) - 1;
	while (i >= 0 && ft_isspace((unsigned char)s[i]))
		i--;
	if (i < 0)
		return (MORE_NONE);
	if (s[i] == '|' || s[i] == '<' || s[i] == '>')
		return (MORE_OP);
	return (MORE_NONE);
}

t_more need_more_input(const char *s)
{
    t_more res;

    res = check_unclosed_quote(s);
    if (res != MORE_NONE)
        return res;
    return check_line_end_operator(s);
}
