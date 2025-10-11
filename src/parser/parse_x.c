/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_x.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:50:46 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/11 10:37:15 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*parse_expr(t_parser *p)
{
	return (parse_and_or(p));
}

static t_node	*create_and_or_node(t_node *left, t_node *right, t_tokentype op)
{
	t_node	*node;

	if (op == TOK_AND_IF)
	{
		node = new_node(ND_AND_IF);
		if (node)
			node->fn = (t_func *)v_and_if();
	}
	else
	{
		node = new_node(ND_OR_IF);
		if (node)
			node->fn = (t_func *)v_or_if();
	}
	if (!node)
	{
		perror("malloc");
		destroy_ast(left);
		destroy_ast(right);
		return (NULL);
	}
	node->u_as.s_bin.left = left;
	node->u_as.s_bin.right = right;
	return (node);
}

t_node	*parse_and_or(t_parser *p)
{
	t_node		*left;
	t_node		*right;
	t_tokentype	op;

	left = parse_pipeline(p);
	while (match(p, TOK_AND_IF) || match(p, TOK_OR_IF))
	{
		op = p->cur->type;
		consume(p);
		right = parse_pipeline(p);
		if (!right)
			return (destroy_ast(left), NULL);
		left = create_and_or_node(left, right, op);
		if (!left)
			return (NULL);
	}
	return (left);
}
