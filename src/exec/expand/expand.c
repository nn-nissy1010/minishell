/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:28:53 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/25 20:18:15 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_expand_word_to_path(t_token *word, char **out_path)
{
	t_buf	w;

	if (!word || !out_path || word->type != TOK_ARG)
		return (2);
	buf_init(&w);
	if (parts_to_buf(word->u.arg.parts, &w) < 0)
		return (buf_free(&w), 2);
	if (split_fields_from_buf(&w, &word->u.arg) < 0)
		return (buf_free(&w), 2);
	buf_free(&w);
	if (word->u.arg.n_items != 1 || !word->u.arg.items || !word->u.arg.items[0]
		|| word->u.arg.items[0][0] == '\0')
	{
		arg_clear_items(&word->u.arg);
		return (1); /* ambiguous redirect */
	}
	*out_path = ft_strdup(word->u.arg.items[0]);
	arg_clear_items(&word->u.arg);
	if (!*out_path)
		return (2);
	return (0);
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
