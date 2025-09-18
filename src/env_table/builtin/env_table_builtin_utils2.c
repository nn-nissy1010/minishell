/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_builtin2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:36:29 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/18 15:03:23 by nnishiya         ###   ########.fr       */
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
    t_env_pair *p;
    size_t dummy;

    p = env_table_find(t, key, &dummy);
    if (!p || !p->key || p->tomb)
        return (0);

    delete_entry(t, p);
    return (1);
}
