/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:55:44 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/26 17:09:26 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int validate_redir_target(t_node *node, t_parser *p)
{
    if (!p->cur || p->cur->type != TOK_ARG) {
        write(2, "syntax error: expected filename/delimiter after redir\n", 54);
        destroy_ast(node);
        return (-1);
    }
    return (0);
}

static void init_redir_struct(t_redir *r, t_tokentype redir_type, t_token *word_tok)
{
    ft_memset(r, 0, sizeof(*r));
    r->kind = redir_type;
    r->word = word_tok;
    if (redir_type == TOK_REDIR_IN || redir_type == TOK_HEREDOC)
        r->fd = 0;
    else
        r->fd = 1;
    r->path = NULL;
    if (redir_type == TOK_HEREDOC) {
        if (word_tok->u.arg.parts &&
            word_tok->u.arg.parts->quote != Q_NONE)
        {
            r->quoted_heredoc = 1;
        }
    }
}

static int append_redir_to_node(t_node *node, t_redir *r)
{
    size_t m;
    t_redir *tmp;

    m = node->as.cmd.n_redirs;
    tmp = ft_realloc(node->as.cmd.redirs,
                 sizeof(t_redir) * (m + 1),
                 sizeof(t_redir) * m);
    if (!tmp) {
        perror("realloc");
        return (-1);
    }
    node->as.cmd.redirs = tmp;
    node->as.cmd.redirs[m] = *r;
    node->as.cmd.n_redirs++;
    return (0);
}

int parse_redir_token(t_node *node, t_parser *p)
{
    t_tokentype redir_type;
    t_redir     r;

    redir_type = p->cur->type;
    consume(p);
    if (validate_redir_target(node, p) < 0)
        return -1;
    init_redir_struct(&r, redir_type, p->cur);

    if (append_redir_to_node(node, &r) < 0) {
        destroy_ast(node);
        return -1;
    }
    consume(p);
    return 0;
}
