/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_builtin_utils3.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 10:12:36 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/13 13:49:28 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	count_cb(const char *k, const char *v, void *ud)
{
	size_t	*count;

	count = (size_t *)ud;
	(*count)++;
	(void)k;
	(void)v;
}

void	fill_cb(const char *k, const char *v, void *ud)
{
	t_envp_builder	*b;
	size_t			key_len;
	size_t			val_len;
	size_t			len;
	char			*entry;

	b = (t_envp_builder *)ud;
	key_len = ft_strlen(k);
	if (v == NULL)
	{
		entry = malloc(key_len + 1);
		if (!entry)
			return ;
		ft_strlcpy(entry, k, key_len + 1);
	}
	else
	{
		val_len = ft_strlen(v);
		len = key_len + 1 + val_len;
		entry = ft_strjoin_3word(ft_strdup(k), "=", v);
		if (!entry)
			return ;
	}
	b->envp[b->n++] = entry;
}
