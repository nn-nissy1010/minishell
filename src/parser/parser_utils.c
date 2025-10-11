/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:48:15 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/11 10:36:11 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	match(t_parser *p, t_tokentype type)
{
	return (p->cur && p->cur->type == type);
}

t_token	*consume(t_parser *p)
{
	t_token	*tok;

	tok = p->cur;
	if (p->cur)
		p->cur = p->cur->next;
	return (tok);
}

t_node	*new_node(t_ntype type)
{
	t_node	*n;

	n = malloc(sizeof(*n));
	if (!n)
		return (NULL);
	n->type = type;
	n->fn = NULL;
	ft_memset(&n->u_as, 0, sizeof(n->u_as));
	return (n);
}
