/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:28:53 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/23 22:21:26 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_redirs(t_redir *rs, size_t n)
{
	size_t	i;
	t_redir	*r;

	i = 0;
	while (i < n)
	{
		r = &rs[i];
		if (!r->word || r->word->type != TOK_ARG)
			return (-1);
		r->path = parts_to_one_word(r->word->u.arg.parts);
		if (!r->path)
			return (-1);
		/* heredoc: r->quoted_heredoc が 1 なら本文側の $ 展開は抑止（別処理で） */
		i++;
	}
	return (0);
}

int	expansion(t_cmd *c)
{
	int	rc;

	if (!c)
		return (3);
	rc = expand_redirs(c->redirs, c->n_redirs);
	if (rc != 0)
		return (rc);
	// rc = expand_argv(c->argv_tokens, &c->argv, &c->argc);
	// if (rc != 0)
	// 	return (rc);
	return (0);
}
