/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_set_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:25:46 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/15 15:38:17 by nnishiya         ###   ########.fr       */
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

int pair_set(t_env_pair *p, const char *k, const char *v){
    char *nk;
    char *nv;
    
    nk = ft_strdup(k);
    if(!nk)
        return -1;
    nv = NULL;
    if(v){ 
        nv=ft_strdup(v);
        if(!nv){
            free(nk); 
            return -1; 
        }
    }
    free(p->key);
    free(p->val);
    p->key=nk;
    p->val=nv;
    p->tomb=0;
    return 0;
}
