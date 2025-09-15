/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_load.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 12:12:15 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/15 13:26:03 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int put_env_entry(t_env_table *t, char *entry)
{
    char *eq = ft_strchr(entry, '=');

    if (!eq)
        return env_table_set(t, entry, NULL);

    size_t klen = (size_t)(eq - entry);
    char *k = (char *)malloc(klen + 1);
    if (!k)
        return -1;

    ft_memcpy(k, entry, klen);
    k[klen] = '\0';

    const char *v = eq + 1;
    int rc = env_table_set(t, k, v);
    free(k);
    return rc;
}

int env_table_load_envp(t_env_table *t, char **envp)
{
    size_t i = 0;

    while (envp && envp[i])
    {
        if (put_env_entry(t, envp[i]) == -1)
            return -1;
        i++;
    }
    return 0;
}
