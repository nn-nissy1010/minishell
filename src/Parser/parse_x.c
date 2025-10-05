/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_x.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:50:46 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/05 09:34:39 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node *parse_expr(t_parser *p)
{
    return parse_and_or(p);
}

t_node *parse_and_or(t_parser *p)
{
    t_node       *left;
    t_node       *right;
    t_node       *node;
    t_tokentype   op;

    left = parse_pipeline(p);
    while (match(p, TOK_AND_IF) || match(p, TOK_OR_IF))
    {
        op = p->cur->type;
        consume(p);
        right = parse_pipeline(p);
        if (!right) 
            return (destroy_ast(left), NULL);
        if (op == TOK_AND_IF)
            node = new_node(ND_AND_IF);
        else
            node = new_node(ND_OR_IF);
        if (!node)
            return (perror("malloc"), destroy_ast(left), destroy_ast(right), NULL);
        node->as.bin.left = left;
        node->as.bin.right = right;
        left = node;
    }
    return (left);
}

t_node *parse_pipeline(t_parser *p)
{
    t_node *left;
    t_node *right;
    t_node *node;

    left = parse_command(p);
    if (!left)
        return (NULL);
    while (match(p, TOK_PIPE))
    {
        consume(p);
        right = parse_command(p);
        if (!right)
        {
            write(STDERR_FILENO, "syntax error near unexpected token '|'\n", 39);
            return (destroy_ast(left), NULL);
        }
        node = new_node(ND_PIPE);
        if (!node)
            return (perror("malloc"), destroy_ast(left), destroy_ast(right), NULL);
        node->fn = v_pipe();      
        node->as.bin.left = left;
        node->as.bin.right = right;
        left = node;
    }
    return (left);
}

t_node *parse_command(t_parser *p)
{
    if (match(p, TOK_LPAR))
        return parse_subshell(p);

    if (match(p, TOK_ARG) || match(p, TOK_REDIR_IN)
        || match(p, TOK_REDIR_OUT) || match(p, TOK_REDIR_APPEND)
        || match(p, TOK_HEREDOC))
        return parse_simple_command(p);
    return (NULL);
}

t_node *parse_subshell(t_parser *p)
{
    t_node *body;
    t_node *node;

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
        destroy_ast(body);
        return (NULL);
    }
    consume(p);
    node = new_node(ND_SUBSHELL);
    if (!node) 
    {
        perror("malloc");
        return (destroy_ast(body), NULL);
    }
    node->as.subshell.body = body;
    return (node);
}
