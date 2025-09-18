/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_find.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:47:37 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/18 11:11:38 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env_pair *probe_slot(t_env_table *t, const char *key, size_t *first_tomb, size_t start)
{
    t_env_pair *p;

    while (1)
    {
        p = &t->pair[start];
        if (!p->key)
        {
            if (*first_tomb != (size_t)-1)
                return (&t->pair[*first_tomb]);
            return (p);
        }
        if (p->tomb && *first_tomb == (size_t)-1)
            *first_tomb = start;
        else if (!p->tomb && ft_strcmp(p->key, key) == 0)
            return (p);
        start = (start + 1) & (t->cap - 1);
        if (start == (fnv1a(key) & (t->cap - 1)))
            return (NULL);
    }
}

t_env_pair *env_table_find(t_env_table *t, const char *key, size_t *first_tomb)
{
    size_t idx;

    if (!t || !t->cap)
        return (NULL);
    idx = fnv1a(key) & (t->cap - 1);
    *first_tomb = (size_t)-1;
    return (probe_slot(t, key, first_tomb, idx));
}
