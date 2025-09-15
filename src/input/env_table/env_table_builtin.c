/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bultin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 12:53:51 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/15 12:54:25 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void env_table_destroy(t_env_table *t)
{
    size_t i;
    if(!t || !t->pair) 
        return;
    for(i=0;i<t->cap;i++)
    { 
        free(t->pair[i].key); 
        free(t->pair[i].val);
    } 
        free(t->pair); 
        t->pair=NULL;
        t->cap=0;
        t->size=0;
}
