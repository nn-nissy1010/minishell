/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:40:43 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/18 11:52:42 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int env_table_put_entry(t_env_table *t, const char *entry)
{
    t_env_pair kv;
    int rc;

    if (parse_entry(entry, &kv) == -1)
        return (-1);
    rc = env_table_set(t, kv.key, kv.val);
    free(kv.key);
    free(kv.val);
    return (rc);
}

int env_table_load_envp(t_env_table *t, char **envp)
{
    size_t i;

    i = 0;
    while (envp && envp[i])
    {
        if (env_table_put_entry(t, envp[i]) == -1)
            return (-1);
        i++;
    }
    return (0);
}

const char *env_table_get(t_env_table *t, const char *key)
{
    t_env_pair *p;
    size_t dummy;

    p = env_table_find(t, key, &dummy);
    if (!p || !p->key || p->tomb)
        return (NULL);
    return (p->val);
}

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
