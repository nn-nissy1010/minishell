/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:18:17 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/11 10:07:12 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_token_to_argbuf(t_token *t, t_argbuf *b)
{
	t_buf	w;

	if (!t || t->type != TOK_ARG)
		return (2);
	buf_init(&w);
	if (parts_to_buf(t->u_u.arg.parts, &w) < 0)
	{
		buf_free(&w);
		return (2);
	}
	if (split_fields_into_argbuf(&w, b) < 0)
	{
		buf_free(&w);
		return (2);
	}
	buf_free(&w);
	return (0);
}

static int	argv_alloc_and_move(t_argbuf *ab, char ***out_argv,
		size_t *out_argc)
{
	size_t	i;
	char	**v;

	if (!ab || !out_argv)
		return (-1);
	if (ab->n + 1 > SIZE_MAX / sizeof(char *))
		return (-1);
	v = (char **)malloc(sizeof(char *) * (ab->n + 1));
	if (!v)
		return (-1);
	i = 0;
	while (i < ab->n)
	{
		v[i] = ab->v[i];
		ab->v[i] = NULL;
		i++;
	}
	v[i] = NULL;
	*out_argv = v;
	if (out_argc)
		*out_argc = ab->n;
	return (0);
}

int	argbuf_to_argv_move(t_argbuf *ab, char ***out_argv, size_t *out_argc)
{
	int	rc;

	rc = argv_alloc_and_move(ab, out_argv, out_argc);
	if (rc == 0)
	{
		free(ab->v);
		ab->v = NULL;
		ab->n = 0;
		ab->cap = 0;
	}
	return (rc);
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
	if (argbuf_to_argv_move(&b, out_argv, out_argc) != 0)
		return (argbuf_free(&b), 2);
	return (0);
}
