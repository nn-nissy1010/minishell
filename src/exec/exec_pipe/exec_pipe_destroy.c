/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_destroy.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:57:40 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/02 17:44:54 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void destroy_pipe_min(t_node *node)
{
    t_node *p = node;

    while (p && p->type == ND_PIPE)
    {
        t_node *left  = p->as.bin.left;
        t_node *right = p->as.bin.right;

        /* 子リンクを切ってから右枝を破棄（再帰破棄でOK） */
        p->as.bin.left  = NULL;
        p->as.bin.right = NULL;

        if (right)
        {
            if (right->fn && right->fn->destroy)
                right->fn->destroy(right);
            else
                free(right);
        }

        /* このパイプノード本体を破棄して左へ進む */
        free(p);
        p = left;
    }

    /* 非PIPE（またはNULL）を最後に破棄 */
    if (p)
    {
        if (p->fn && p->fn->destroy)
            p->fn->destroy(p);
        else
            free(p);
    }
}


