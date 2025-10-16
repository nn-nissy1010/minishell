/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:57:45 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/16 10:41:24 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_more	scan_single_quote(const char *s, size_t *i)
{
	(*i)++;
	while (s[*i] && s[*i] != '\'')
		(*i)++;
	if (!s[*i])
		return (MORE_QUOTE_S);
	return (MORE_NONE);
}

static t_more	scan_double_quote(const char *s, size_t *i)
{
	(*i)++;
	while (s[*i] && s[*i] != '"')
	{
		if (s[*i] == '\\' && (s[*i + 1] == '"' || s[*i + 1] == '\\'
				|| s[*i + 1] == '$'))
			(*i)++;
		(*i)++;
	}
	if (!s[*i])
		return (MORE_QUOTE_D);
	return (MORE_NONE);
}

static t_more	check_unclosed_quote(const char *s)
{
	size_t	i;
	t_more	res;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'')
		{
			res = scan_single_quote(s, &i);
			if (res != MORE_NONE)
				return (res);
		}
		else if (s[i] == '"')
		{
			res = scan_double_quote(s, &i);
			if (res != MORE_NONE)
				return (res);
		}
		else if (s[i] == '\\' && !s[i + 1])
			return (MORE_BSLASH);
		i++;
	}
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
	{
		if (i > 1 && s[i - 1] == s[i] && s[i - 2] == s[i])
			return (MORE_NONE);
		if (i > 0 && s[i - 1] == s[i])
			return (MORE_OP);
		return (MORE_OP);
	}
	if (s[i] == '&')
	{
		if (i > 1 && s[i - 1] == '&' && s[i - 2] == '&')
			return (MORE_NONE);
		if (i > 0 && s[i - 1] == '&')
			return (MORE_OP);
	}
	return (MORE_NONE);
}

t_more	need_more_input(const char *s)
{
	t_more	res;

	res = check_unclosed_quote(s);
	if (res != MORE_NONE)
		return (res);
	return (check_line_end_operator(s));
}
