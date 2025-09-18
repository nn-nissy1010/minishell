/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:40:43 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/18 15:05:16 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_env_table(void)
{
    env_table_foreach(&g_env, print_cb, NULL);
    return 0;
}

const char *search_env_table(const char *key)
{
    size_t dummy;
    t_env_pair *p;

    p = env_table_find(&g_env, key, &dummy);
    if (!p || !p->key || p->tomb)
        return (NULL);
    return (p->val);
}
int update_env_table(const char *key, const char *value)
{
    char *entry;
    int ret;

    if (!key || !value)
        return (1);

    entry = ft_strjoin_3word(ft_strdup(key), "=", value);
    if (!entry)
        return (1);
    ret = env_table_put_entry(&g_env, entry);
    free(entry);
    return (ret == -1);
}

int unset_env_table(const char *arg)
{
    if (env_table_unset(&g_env, arg))
        return (0);
    else
        return (1);
}
