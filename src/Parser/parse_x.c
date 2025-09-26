/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_x.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:50:46 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/26 16:51:17 by nnishiya         ###   ########.fr       */
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
        node = new_node(op == TOK_AND_IF ? ND_AND_IF : ND_OR_IF);
        if (!node) {
            perror("malloc");
            destroy_ast(left);
            destroy_ast(right);
            return NULL;
        }
        node->as.bin.left = left;
        node->as.bin.right = right;
        left = node;
    }
    return left;
}

t_node *parse_pipeline(t_parser *p)
{
    t_node *left;
    t_node *right;
    t_node *node;

    left = parse_command(p);
    while (match(p, TOK_PIPE))
    {
        consume(p);
        right = parse_command(p);
        node = new_node(ND_PIPE);
        if (!node) {
            perror("malloc");
            destroy_ast(left);
            destroy_ast(right);
            return NULL;
        }
        node->as.bin.left = left;
        node->as.bin.right = right;
        left = node;
    }
    return left;
}


t_node *parse_command(t_parser *p)
{
    if (match(p, TOK_LPAR))
        return parse_subshell(p);
    else
        return parse_simple_command(p);
}

t_node *parse_subshell(t_parser *p)
{
    t_node *body;
    t_node *node;

    consume(p);
    body = parse_expr(p);
    if (!body) {
        fprintf(stderr, "syntax error: invalid subshell body\n");
        return NULL;
    }
    if (!match(p, TOK_RPAR)) {
        fprintf(stderr, "syntax error: expected ')'\n");
        destroy_ast(body);
        return NULL;
    }
    consume(p);
    node = new_node(ND_SUBSHELL);
    if (!node) {
        perror("malloc");
        destroy_ast(body);
        return NULL;
    }
    node->as.subshell.body = body;
    return node;
}
