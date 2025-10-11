/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:28:53 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/09 12:39:28 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	glob_resolve_for_redir(const char *cand, char **out)
{
	t_argbuf	m;
	int			rc;

	if (!cand || !out)
		return (2);
	if (!should_glob_expand(cand))
	{
		*out = ft_strdup(cand);
		if (!*out)
			return (2);
		unmask_globs(*out);
		return (0);
	}
	argbuf_init(&m);
	rc = expand_glob_pattern(cand, &m);
	if (rc < 0)
	{
		argbuf_free(&m);
		return (2);
	}
	return (resolve_from_matches(cand, &m, out));
}

int	redir_expand_word_to_path(t_token *word, char **out_path)
{
	char	*cand;
	int		rc;

	cand = NULL;
	rc = expand_word_to_single_field(word, &cand);
	if (rc == 1)
		return (1);
	if (rc != 0)
		return (2);
	rc = glob_resolve_for_redir(cand, out_path);
	free(cand);
	return (rc);
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
		free(r->path);
		r->path = NULL;
		rc = redir_expand_word_to_path(r->word, &r->path);
		if (rc == 1)
		{
			err3("minishell: ", "ambiguous redirect", "\n");
			set_exit_status(1);
			return (1);
		}
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
