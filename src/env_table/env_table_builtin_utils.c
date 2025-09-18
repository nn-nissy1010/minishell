/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_builtin_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 16:04:55 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/18 10:48:12 by nnishiya         ###   ########.fr       */
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
