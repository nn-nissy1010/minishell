/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_set.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:21:15 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/02 11:29:27 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int pair_set(t_env_pair *p, const char *k, const char *v)
{
    char *nk;
    char *nv;
    
    nk = ft_strdup(k);
    if(!nk)
        return -1;
    nv = NULL;
    if(v)
    { 
        nv=ft_strdup(v);
        if(!nv)
            return (free(nk), -1);
    }
    free(p->key);
    free(p->val);
    p->key=nk;
    p->val=nv;
    p->tomb=0;
    return (0);
}

int env_table_set(t_env_table *t, const char *key, const char *val)
{
    t_env_pair *p;
    size_t first_tomb;

    if (!key || !*key)
        return (0);
    if (ensure_grow(t) == -1)
        return (-1);

    p = env_table_find(t, key, &first_tomb);
    if (!p)
        return (-1);

    if (pair_set(p, key, val) == -1)
        return (-1);

    if (!p->key || p->tomb)
        t->size++;
    return (0);
}
