/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_parts_params.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 22:17:48 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/25 20:50:48 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	read_name_simple(const char *s, size_t i, char *name,
		size_t cap)
{
	size_t	j;
	size_t	n;

	j = i;
	if (!(ft_isalpha((unsigned char)s[j]) || s[j] == '_'))
		return (0);
	j++;
	while (s[j] && (ft_isalnum((unsigned char)s[j]) || s[j] == '_'))
		j++;
	n = j - i;
	if (n >= cap)
		n = cap - 1;
	if (n)
		ft_memcpy(name, s + i, n);
	name[n] = '\0';
	return (j - i);
}

static int	expand_dollar_simple(const char *s, size_t *i, t_buf *b)
{
	char	name[256];
	size_t	adv;

	adv = read_name_simple(s, *i, name, sizeof(name));
	if (adv == 0)
		return (buf_putc(b, '$'));
	*i += adv;
	return (emit_env(b, name));
}

int	expand_dollar(const char *s, size_t *i, t_buf *b)
{
	(*i)++;
	if (!s[*i])
		return (buf_putc(b, '$'));
	if (s[*i] == '?')
	{
		(*i)++;
		return (emit_status(b));
	}
	return (expand_dollar_simple(s, i, b));
}

char	*expand_params_part(const char *s)
{
	t_buf	b;
	size_t	i;
	char	*out;

	if (!s)
		return (NULL);
	buf_init(&b);
	i = 0;
	while (s[i])
	{
		if (s[i] != '$')
		{
			if (buf_putc(&b, s[i]) < 0)
				return (buf_free(&b), NULL);
			i++;
			continue ;
		}
		if (expand_dollar(s, &i, &b) < 0)
			return (buf_free(&b), NULL);
	}
	if (!b.data)
		return (ft_strdup(""));
	out = b.data;
	b.data = NULL;
	return (buf_free(&b), out);
}
