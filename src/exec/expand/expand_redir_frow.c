/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir_frow.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:28:53 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/16 16:02:19 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	resolve_from_matches(const char *cand, t_argbuf *m, char **out)
{
	if (m->n == 0)
	{
		*out = ft_strdup(cand);
		argbuf_free(m);
		if (!*out)
			return (2);
		unmask_globs(*out);
		return (0);
	}
	if (m->n == 1)
	{
		*out = m->v[0];
		m->v[0] = NULL;
		argbuf_free(m);
		return (0);
	}
	argbuf_free(m);
	return (1);
}

int	make_word_buf(t_token *word, t_buf *w)
{
	if (!word || !w || word->type != TOK_ARG)
		return (2);
	buf_init(w);
	if (parts_to_buf(word->u_u.arg.parts, w) < 0)
	{
		buf_free(w);
		return (2);
	}
	return (0);
}

int	buf_to_argbuf(t_buf *w, t_argbuf *ab)
{
	if (!w || !ab)
		return (2);
	argbuf_init(ab);
	if (split_fields_into_argbuf(w, ab) < 0)
	{
		argbuf_free(ab);
		return (2);
	}
	return (0);
}

int	take_single_field_and_dup(t_argbuf *ab, char **out)
{
	char	*dup;

	if (!ab || !out)
		return (2);
	*out = NULL;
	if (ab->n == 1 && ab->v && ab->v[0] && ab->v[0][0] != '\0')
	{
		dup = ft_strdup(ab->v[0]);
		argbuf_free(ab);
		if (!dup)
			return (2);
		*out = dup;
		return (0);
	}
	argbuf_free(ab);
	return (1);
}

int	expand_word_to_single_field(t_token *word, char **out)
{
	t_buf		w;
	t_argbuf	ab;
	int			rc;

	rc = make_word_buf(word, &w);
	if (rc != 0)
		return (2);
	rc = buf_to_argbuf(&w, &ab);
	if (rc != 0)
	{
		buf_free(&w);
		return (rc);
	}
	rc = take_single_field_and_dup(&ab, out);
	buf_free(&w);
	return (rc);
}
