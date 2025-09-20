/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:40:43 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/19 16:12:22 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_env_table(void)
{
    t_env_table *table;
	table = env_table();
    env_table_foreach(table, print_cb, NULL);
    return 0;
}

const char *search_env_table(const char *key)
{
    size_t dummy;
    t_env_pair *p;
    t_env_table *table;
	table = env_table();

    p = env_table_find(table, key, &dummy);
    if (!p || !p->key || p->tomb)
        return (NULL);
    return (p->val);
}
int update_env_table(const char *key, const char *value)
{
    char *entry;
    int ret;

    t_env_table *table;
	table = env_table();

    if (!key || !value)
        return (1);

    entry = ft_strjoin_3word(ft_strdup(key), "=", value);
    if (!entry)
        return (1);
    ret = env_table_put_entry(table, entry);
    free(entry);
    return (ret == -1);
}

int unset_env_table(const char *arg)
{
    t_env_table *table;
	table = env_table();
    
    if (env_table_unset(table, arg))
        return (0);
    else
        return (1);
}
