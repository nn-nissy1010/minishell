/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_builtin_utils2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 16:04:55 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/15 16:08:13 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_cb(const char *k, const char *v, void *ud)
{
    (void)ud;

    if (v) {
        printf("%s=%s\n", k, v);
    } else {
        printf("%s=\n", k);
    }
}

void env_table_foreach(const t_env_table *t, t_env_iter_cb cb, void *ud)
{
    size_t i;

    i = 0;
    while (i < t->cap)
    {
        if (t->pair[i].key && !t->pair[i].tomb)
            cb(t->pair[i].key, t->pair[i].val, ud);
        i++;
    }
}

int env_table_put_entry(t_env_table *t, const char *entry)
{
    const char *eq;
    size_t key_len;
    char *key;
    const char *value;
    int rc;

    eq = ft_strchr(entry, '=');
    if (!eq)
        return (env_table_set(t, entry, NULL));

    key_len = (size_t)(eq - entry);
    key = (char *)malloc(key_len + 1);
    if (!key)
        return (-1);

    ft_memcpy(key, entry, key_len);
    key[key_len] = '\0';

    value = eq + 1;
    rc = env_table_set(t, key, value);

    free(key);
    return (rc);
}
