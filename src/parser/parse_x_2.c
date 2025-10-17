/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_x_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:50:46 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/16 18:00:08 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*parse_pipeline(t_parser *p)
{
	t_node	*left;
	t_node	*right;
	t_node	*node;

	left = parse_command(p);
	if (!left)
		return (NULL);
	while (match(p, TOK_PIPE))
	{
		consume(p);
		right = parse_command(p);
		if (!right)
			return (write(2, "syntax error near unexpected token '|'\n", 39),
				destroy_ast(left), NULL);
		node = new_node(ND_PIPE);
		if (!node)
			return (perror("malloc"), destroy_ast(left), destroy_ast(right),
				NULL);
		node->fn = v_pipe();
		node->u_as.s_bin.left = left;
		node->u_as.s_bin.right = right;
		left = node;
	}
	return (left);
}

t_node	*parse_command(t_parser *p)
{
	if (match(p, TOK_LPAR))
		return (parse_subshell(p));
	if (match(p, TOK_ARG) || match(p, TOK_REDIR_IN) || match(p, TOK_REDIR_OUT)
		|| match(p, TOK_REDIR_APPEND) || match(p, TOK_HEREDOC))
		return (parse_simple_command(p));
	return (NULL);
}

t_node	*parse_subshell(t_parser *p)
{
	t_node	*body;
	t_node	*node;

	consume(p);
	body = parse_expr(p);
	if (!body)
	{
		write(STDERR_FILENO, "syntax error: invalid subshell body\n", 36);
		return (NULL);
	}
	if (!match(p, TOK_RPAR))
	{
		write(STDERR_FILENO, "syntax error: expected ')'\n", 27);
		return (destroy_ast(body), NULL);
	}
	consume(p);
	node = new_node(ND_SUBSHELL);
	if (!node)
	{
		perror("malloc");
		return (destroy_ast(body), NULL);
	}
	node->u_as.s_subshell.body = body;
	node->fn = (t_func *)v_subshell();
	return (node);
}
