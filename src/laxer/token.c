/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:36:39 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/06 15:40:20 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *new_token_n(t_tokentype type, const char *s, size_t n)
{
    t_token *t;

    t = ft_calloc(1, sizeof(t_token));
    if (!t)
        return (perror("malloc_error"), NULL);
    t->type = type;
    t->next = NULL;

    if (type == TOK_ARG)
    {
        if (s && n)
            t->u.arg.raw = ft_strndup(s, n);
        else if (s)
            t->u.arg.raw = ft_strdup("");
        else
            t->u.arg.raw = NULL;
        t->u.arg.parts = NULL;
        t->u.arg.expanded = 0;
    }
    else if (type == TOK_HEREDOC)
    {
        t->u.heredoc.heredoc_quoted = 0;
    }

    return (t);
}

t_token *new_token(t_tokentype type, const char *s)
{
    if (!s)
        return new_token_n(type, NULL, 0);
    return new_token_n(type, s, ft_strlen(s));
}

void tok_push(t_token **head, t_token **tail, t_token *node)
{
    if (!node)
        return;
    if (!*head)
        *head = node;
    else
        (*tail)->next = node;
    *tail = node;
}

void free_tokens(t_token *t)
{
    t_token     *n; 

      while (t)
    {
        n = t->next;
        destroy_token(t);  
        t = n;
    }
}
