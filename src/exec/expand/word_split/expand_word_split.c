/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word_split.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:59:20 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/25 20:14:39 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	alloc_items_vec(char ***out, size_t n)
{
	*out = (char **)malloc(sizeof(char *) * (n + 1));
	if (*out == NULL)
		return (-1);
	return (0);
}

static void	free_items_partial(char **items, size_t k)
{
	size_t	t;

	t = 0;
	while (t < k)
	{
		free(items[t]);
		t++;
	}
	free(items);
}

static int	alloc_and_copy_field(const t_buf *b, size_t i, char **out,
		size_t *next)
{
	size_t	len;
	char	*s;

	len = field_span(b, i);
	s = (char *)malloc(len + 1);
	if (!s)
		return (-1);
	if (len)
		ft_memcpy(s, b->data + i, len);
	s[len] = '\0';
	*out = s;
	*next = skip_unquoted_ifs(b, i + len);
	return (0);
}

int	split_fields_from_buf(const t_buf *b, t_arg *arg)
{
	char	**items;
	size_t	i;
	size_t	k;
	size_t	n;

	if (!b || !arg)
		return (-1);
	arg_clear_items(arg);
	n = count_fields_from_buf(b);
	if (n == 0)
		return (0);
	if (alloc_items_vec(&items, n) < 0)
		return (-1);
	i = skip_unquoted_ifs(b, 0);
	k = 0;
	while (k < n)
	{
		if (alloc_and_copy_field(b, i, &items[k], &i) < 0)
			return (free_items_partial(items, k), -1);
		k++;
	}
	items[n] = NULL;
	arg->items = items;
	arg->n_items = n;
	return (0);
}
