/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_parse.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:38:50 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/11 09:33:44 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_entry(const char *entry, t_env_pair *out)
{
	const char	*eq;
	size_t		klen;

	eq = ft_strchr(entry, '=');
	out->tomb = 0;
	if (!eq)
	{
		out->key = ft_strdup(entry);
		if (!out->key)
			return (-1);
		out->val = NULL;
		return (0);
	}
	klen = (size_t)(eq - entry);
	out->key = (char *)malloc(klen + 1);
	if (!out->key)
		return (-1);
	ft_memcpy(out->key, entry, klen);
	out->key[klen] = '\0';
	out->val = ft_strdup(eq + 1);
	if (!out->val)
		return (free(out->key), -1);
	return (0);
}
