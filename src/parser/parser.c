/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 20:04:48 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/11 10:35:48 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_arg_token(t_node *node, t_parser *p)
{
	size_t	n;
	t_token	**tmp;

	n = node->u_as.cmd.n_argv_tokens;
	tmp = ft_realloc(node->u_as.cmd.argv_tokens, sizeof(t_token *) * n,
			sizeof(t_token *) * (n + 1));
	if (!tmp)
	{
		perror("realloc");
		destroy_ast(node);
		return (-1);
	}
	node->u_as.cmd.argv_tokens = tmp;
	node->u_as.cmd.argv_tokens[n] = p->cur;
	node->u_as.cmd.n_argv_tokens++;
	consume(p);
	return (0);
}

static void	init_simple_command_node(t_node *node)
{
	node->u_as.cmd.redirs = NULL;
	node->u_as.cmd.n_redirs = 0;
	node->u_as.cmd.argv_tokens = NULL;
	node->u_as.cmd.n_argv_tokens = 0;
	node->u_as.cmd.argv = NULL;
	node->u_as.cmd.argc = 0;
}

static int	parse_command_body(t_node *node, t_parser *p)
{
	while (p->cur && p->cur->type != TOK_PIPE && p->cur->type != TOK_AND_IF
		&& p->cur->type != TOK_OR_IF && p->cur->type != TOK_RPAR
		&& p->cur->type != TOK_EOF)
	{
		if (p->cur->type == TOK_ARG)
		{
			if (parse_arg_token(node, p) < 0)
				return (-1);
		}
		else if (p->cur->type == TOK_REDIR_IN || p->cur->type == TOK_REDIR_OUT
			|| p->cur->type == TOK_REDIR_APPEND || p->cur->type == TOK_HEREDOC)
		{
			if (parse_redir_token(node, p) < 0)
				return (-1);
		}
		else
			break ;
	}
	return (0);
}

t_node	*parse_simple_command(t_parser *p)
{
	t_node	*node;

	node = new_node(ND_COMMAND);
	if (!node)
		return (NULL);
	node->fn = v_cmd();
	init_simple_command_node(node);
	if (parse_command_body(node, p) < 0)
	{
		destroy_ast(node);
		return (NULL);
	}
	return (node);
}

t_node	*parse(t_token *toklist)
{
	t_parser	p;
	t_node		*root;

	p.cur = toklist;
	root = parse_expr(&p);
	if (!match(&p, TOK_EOF))
	{
		write(2, "syntax error near unexpected token\n", 35);
		destroy_ast(root);
		return (NULL);
	}
	return (root);
}
