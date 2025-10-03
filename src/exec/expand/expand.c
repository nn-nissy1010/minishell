/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:28:53 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/03 00:48:11 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int expand_word_to_single_field(t_token *word, char **out)
{
    t_buf    w;
    t_argbuf ab;
    size_t   i;

    if (!word || !out || word->type != TOK_ARG)
        return 2;

    *out = NULL;
    buf_init(&w);
    argbuf_init(&ab);

    if (parts_to_buf(word->u.arg.parts, &w) < 0)
	{
        buf_free(&w);
        return 2;
    }
    if (split_fields_into_argbuf(&w, &ab) < 0) 
	{
        buf_free(&w);

        i = 0;
        while (i < ab.n) 
		{ 
			if (ab.v[i]) 
				free(ab.v[i]); 
			i++; 
		}
        free(ab.v);
        return 2;
    }
    buf_free(&w);

    if (ab.n != 1 || !ab.v || !ab.v[0] || ab.v[0][0] == '\0') 
	{
        i = 0;
        while (i < ab.n) 
		{ 
			if (ab.v[i]) 
				free(ab.v[i]); 
			i++; 
		}
        free(ab.v);
        return 1; 
    }
    *out = ft_strdup(ab.v[0]);
    i = 0;
    while (i < ab.n)
	 { 
		if (ab.v[i]) 
			free(ab.v[i]); 
		i++;
	 }
    free(ab.v);
    if (!*out) 
		return 2;
    return 0;
}

int redir_expand_word_to_path(t_token *word, char **out_path)
{
    int rc;

    rc = expand_word_to_single_field(word, out_path);
    if (rc == 0) 
		return 0;
    if (rc == 1) 
		return 1; 
    return 2;              
}


int	expand_redirs(t_redir *rs, size_t n)
{
	size_t	i;
	t_redir	*r;
	int		rc;

	i = 0;
	while (i < n)
	{
		r = &rs[i];
		if (!r->word || r->word->type != TOK_ARG)
			return (2);
		if (r->path)
		{
			free(r->path);
			r->path = NULL;
		}
		rc = redir_expand_word_to_path(r->word, &r->path);
		if (rc != 0)
			return (rc);
		i++;
	}
	return (0);
}

int	expand_argv_cmd(t_cmd *c)
{
	int		rc;
	size_t	i;

	if (!c)
		return (2);
	if (c->argv)
	{
		i = 0;
		while (i < c->argc)
		{
			free(c->argv[i]);
			i++;
		}
		free(c->argv);
		c->argv = NULL;
		c->argc = 0;
	}
	rc = expand_argv(c->argv_tokens, c->n_argv_tokens, &c->argv, &c->argc);
	return (rc);
}

int	expansion(t_cmd *c)
{
	int	rc;

	if (!c)
		return (2);
	rc = expand_redirs(c->redirs, c->n_redirs);
	if (rc != 0)
		return (set_exit_status(1), -1);
	rc = expand_argv_cmd(c);
	if (rc != 0)
		return (set_exit_status(1), -1);
	return (0);
}
