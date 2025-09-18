/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_core.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:36:06 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/18 10:59:27 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned long fnv1a(const char *s)
{
    uint64_t hash;

    hash = 1469598103934665603ULL;
    while (*s)
    {
        hash ^= (unsigned char)*s;
        hash *= 1099511628211ULL;
        s++;
    }
    return (unsigned long)hash;
}

static int move_entry(t_env_table *t, t_env_pair *p)
{
    size_t idx;

    idx = fnv1a(p->key) & (t->cap - 1);
    while (t->pair[idx].key && !t->pair[idx].tomb)
        idx = (idx + 1) & (t->cap - 1);
    if (pair_set(&t->pair[idx], p->key, p->val) == -1)
    {
        free(p->key);
        free(p->val);
        return (-1);
    }
    t->size++;
    free(p->key);
    free(p->val);
    return (0);
}

static int rehash(t_env_table *t, size_t newcap)
{
    size_t i;
    t_env_pair *old;
    size_t oldcap;
    t_env_pair *na;

    old = t->pair;
    oldcap = t->cap;
    na = (t_env_pair *)ft_calloc(newcap, sizeof(t_env_pair));
    if (!na)
        return (-1);
    t->pair = na;
    t->cap = newcap;
    t->size = 0;
    i = 0;
    while (i < oldcap)
    {
        if (old[i].key && !old[i].tomb)
            if (move_entry(t, &old[i]) == -1)
                return (-1);
        i++;
    }
    free(old);
    return (0);
}

int ensure_grow(t_env_table *t)
{
    size_t newcap;

    if ((t->size + 1) * 3 >= t->cap * 2)
    {
        if (t->cap)
            newcap = t->cap << 1;
        else
            newcap = 8;
        return (rehash(t, newcap));
    }
    return (0);
}
