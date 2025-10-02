/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parts_join.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:59:07 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/02 11:27:17 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t parts_total_len(t_arg_part *parts)
{
    size_t      total;
    t_arg_part *p;

    total = 0;
    p = parts;
    while (p)
    {
        total += ft_strlen(p->text);
        p = p->next;
    }
    return total;
}

static void copy_parts_to_buf(char *res, t_arg_part *parts)
{
    size_t      pos;
    size_t      len;
    t_arg_part *p;

    pos = 0;
    p = parts;
    while (p)
    {
        len = ft_strlen(p->text);
        ft_memcpy(res + pos, p->text, len);
        pos += len;
        p = p->next;
    }
    res[pos] = '\0';
}

static char *join_parts(t_arg_part *parts)
{
    char   *res;
    size_t  total;

    total = parts_total_len(parts);
    res = malloc(total + 1);
    if (!res)
        return (NULL);
    copy_parts_to_buf(res, parts);
    return (res);
}

int finalize_arg(t_arg *arg)
{
    arg->raw = join_parts(arg->parts);
    return (arg->raw != NULL);
}
