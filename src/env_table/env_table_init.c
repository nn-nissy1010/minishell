/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 11:30:55 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/02 11:29:20 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t next_pow2(size_t x)
{
    size_t p;
    size_t target;
    
    p = 1;
    if (x < 8)
        target = 8;
    else
        target = x;
    while (p < target)
        p <<= 1;
    return p;
}

int env_table_init(t_env_table *t, size_t cap_hint)
{
    size_t cap;
    
    cap = next_pow2(cap_hint);
    t->pair = (t_env_pair*)ft_calloc(cap, sizeof(t_env_pair));
    if(!t->pair)
        return (-1);
    t->cap=cap;
    t->size=0;
    return (0);
}
