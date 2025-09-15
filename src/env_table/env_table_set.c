/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_set.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:21:15 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/15 13:44:02 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int ensure_grow(t_env_table *t)
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


static int set_pair(t_set_ctx *ctx, t_env_pair *p)
{
    if (!p->key)
    {
        if (ctx->first_tomb != (size_t)-1)
            p = &ctx->t->pair[ctx->first_tomb];
        if (pair_set(p, ctx->key, ctx->val) == -1)
            return (-1);
        ctx->t->size++;
        return (1);
    }
    if (p->tomb)
    {
        if (ctx->first_tomb == (size_t)-1)
            ctx->first_tomb = (size_t)(p - ctx->t->pair);
    }
    else if (ft_strcmp(p->key, ctx->key) == 0)
    {
        if (pair_set(p, ctx->key, ctx->val) == -1)
            return (-1);
        return (1);
    }
    return (0);
}


int env_table_set(t_env_table *t, const char *key, const char *val)
{
    t_set_ctx ctx;
    int rc;

    if (!key || !*key)
        return (0);
    if (ensure_grow(t) == -1)
        return (-1);

    ctx.t = t;
    ctx.key = key;
    ctx.val = val;
    ctx.first_tomb = (size_t)-1;
    ctx.idx = fnv1a(key) & (t->cap - 1);

    while (1)
    {
        rc = set_pair(&ctx, &t->pair[ctx.idx]);
        if (rc != 0)
        {
            if (rc == 1)
                return (0);
            return (-1);
        }
        ctx.idx = (ctx.idx + 1) & (t->cap - 1);
    }
}
