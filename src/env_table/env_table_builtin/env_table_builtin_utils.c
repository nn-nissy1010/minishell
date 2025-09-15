/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_builtin_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:36:29 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/15 16:08:10 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_entry(t_env_pair *p)
{
    free(p->key);
    free(p->val);
    p->key = NULL;
    p->val = NULL;
}

static int delete_entry(t_env_table *t, t_env_pair *p)
{
    free_entry(p);
    p->tomb = 1;
    t->size--;
    return (1);
}

int env_table_unset(t_env_table *t, const char *key)
{
    unsigned long h;
    size_t idx;
    size_t start;
    t_env_pair *p;

    if (t->cap == 0)
        return (0);
    h = fnv1a(key);
    idx = h & (t->cap - 1);
    start = idx;
    while (1)
    {
        p = &t->pair[idx];
        if (!p->key)
        {
            if (!p->tomb)
                return (0);
        }
        else if (!p->tomb && ft_strcmp(p->key, key) == 0)
            return (delete_entry(t, p));
        idx = (idx + 1) & (t->cap - 1);
        if (idx == start)
            return (0);
    }
}
