/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_builtin_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 16:04:55 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/13 10:58:25 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_cb(const char *k, const char *v, void *ud)
{
	(void)ud;
	if (v)
	{
		printf("%s=%s\n", k, v);
	}
	else
	{
		printf("%s=\n", k);
	}
}

void	env_table_foreach(const t_env_table *t, t_env_iter_cb cb, void *ud)
{
	size_t	i;

	i = 0;
	while (i < t->cap)
	{
		if (t->pair[i].key && !t->pair[i].tomb)
			cb(t->pair[i].key, t->pair[i].val, ud);
		i++;
	}
}

int	env_table_put_entry(t_env_table *t, const char *entry)
{
	t_env_pair	kv;
	int			rc;

	if (parse_entry(entry, &kv) == -1)
		return (-1);
	rc = env_table_set(t, kv.key, kv.val);
	free(kv.key);
	free(kv.val);
	return (rc);
}

int	env_table_load_envp(t_env_table *t, char **envp)
{
	size_t	i;

	i = 0;
	while (envp && envp[i])
	{
		if (env_table_put_entry(t, envp[i]) == -1)
			return (-1);
		i++;
	}
	return (0);
}

const char	*env_table_get(t_env_table *t, const char *key)
{
	t_env_pair	*p;
	size_t		dummy;

	p = env_table_find(t, key, &dummy);
	if (!p || !p->key || p->tomb)
		return (NULL);
	return (p->val);
}
