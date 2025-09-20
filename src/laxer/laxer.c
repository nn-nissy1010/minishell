/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   laxer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:36:28 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/19 16:15:19 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *new_token_n(t_tokentype type, const char *s, size_t n)
{
    t_token *t;

    t = malloc(sizeof(*t));
    if (!t)
        return (perror("malloc_error"), NULL);
    t->type = type;
    t->next = NULL;
    if (type == TOK_ARG)
    {
        if (s && n)
            t->u.arg.raw = ft_strndup(s, n);
        else
            t->u.arg.raw = ft_strdup("");
        t->u.arg.items = NULL;
        t->u.arg.n_items = 0;
        t->u.arg.has_param = 0;
        t->u.arg.saw_quote = 0;
        t->u.arg.has_unq_glob = 0;
        t->u.arg.expanded = 0;
    }
    else if (type == TOK_HEREDOC)
        t->u.heredoc.heredoc_quoted = 0;
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
    t_token *n;
    size_t   i;

    while (t)
    {
        n = t->next;
        if (t->type == TOK_ARG)
        {
            free(t->u.arg.raw);
            if (t->u.arg.items)
            {
                i = 0;
                while (i < t->u.arg.n_items)
                {
                    free(t->u.arg.items[i]);
                    i++;
                }
                free(t->u.arg.items);
            }
        }
        free(t);
        t = n;
    }
}

t_token *lexer(const char *input)
{
    const char *p;
    const char *start;
    t_token *head;
    t_token *tail;
    
    p = input;
    head = NULL;
    tail = NULL;

    while (*p)
    {
        while (*p && ft_isspace((unsigned char)*p))
            p++;
        if (!*p)
            break;
        start = p;
        while (*p && !ft_isspace((unsigned char)*p))
            p++;

        size_t len = p - start;
        t_token *tok = new_token_n(TOK_ARG, start, len);
        tok_push(&head, &tail, tok);
    }
    tok_push(&head, &tail, new_token(TOK_EOF, NULL));
    return head;
}
