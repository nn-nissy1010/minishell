/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 12:53:51 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/15 15:58:30 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int get_env_table(void)
{
    env_table_foreach(&g_env, print_cb, NULL);
    return 0;
}

int update_env_table(const char *arg)
{
    if (env_table_put_entry(&g_env, arg) == -1)
        return (1);
    return (0);
}

int unset_env_table(const char *arg)
{
    if (env_table_unset(&g_env, arg))
        return (0);
    else
        return (1);
}

void destroy_env_table(t_env_table *t)
{
    size_t i;

    if (!t || !t->pair)
        return;
    i = 0;
    while (i < t->cap)
    {
        free_entry(&t->pair[i]);
        i++;
    }
    free(t->pair);
    t->pair = NULL;
    t->cap = 0;
    t->size = 0;
}
