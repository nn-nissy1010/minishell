/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:18:17 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/25 19:49:38 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	token_parts_to_items(t_token *tok)
{
	t_buf	w;

	if (!tok || tok->type != TOK_ARG)
		return (2);
	buf_init(&w);
	if (parts_to_buf(tok->u.arg.parts, &w) < 0)
		return (buf_free(&w), 2);
	if (split_fields_from_buf(&w, &tok->u.arg) < 0)
	{
		buf_free(&w);
		arg_clear_items(&tok->u.arg);
		return (2);
	}
	buf_free(&w);
	return (0);
}

static int	process_token_to_argbuf(t_token *t, t_argbuf *b)
{
	size_t	j;

	if (token_parts_to_items(t) != 0)
		return (2);
	j = 0;
	while (j < t->u.arg.n_items)
	{
		if (argbuf_push(b, t->u.arg.items[j]) != 0)
		{
			arg_clear_items(&t->u.arg);
			return (2);
		}
		j++;
	}
	arg_clear_items(&t->u.arg);
	return (0);
}

int	expand_argv(t_token **argv_tokens, size_t n_tokens, char ***out_argv,
		size_t *out_argc)
{
	t_argbuf	b;
	size_t		i;
	t_token		*t;

	if (!out_argv || !out_argc)
		return (2);
	argbuf_init(&b);
	if (!argv_tokens)
		n_tokens = 0;
	i = 0;
	while (i < n_tokens)
	{
		t = argv_tokens[i];
		if (t && t->type == TOK_ARG)
		{
			if (process_token_to_argbuf(t, &b) != 0)
				return (argbuf_free(&b), 2);
		}
		i++;
	}
	if (argbuf_terminate(&b) != 0)
		return (argbuf_free(&b), 2);
	*out_argv = b.v;
	*out_argc = b.n;
	return (0);
}
