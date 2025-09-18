/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_destroy.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 15:03:25 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/18 15:03:35 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
