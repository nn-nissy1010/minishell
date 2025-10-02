/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:48:15 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/02 11:25:21 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int match(t_parser *p, t_tokentype type)
{
    return (p->cur && p->cur->type == type);
}

t_token *consume(t_parser *p)
{
    t_token *tok;
    tok = p->cur;
    if (p->cur)
        p->cur = p->cur->next;
    return (tok);
}

t_node *new_node(t_ntype type)
{
    t_node *n;

    n = malloc(sizeof(*n));
    if (!n)
        return (NULL);
    n->type = type;
    n->fn = NULL; // TODO: 後で exec/destroy テーブルを差し込む
    ft_memset(&n->as, 0, sizeof(n->as));
    return (n);
}
