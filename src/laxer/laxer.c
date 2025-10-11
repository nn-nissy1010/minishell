/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   laxer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:36:28 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/11 10:18:12 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*scan_arg(const char **pp)
{
	t_arg_part	*parts;
	t_arg_part	*part;
	t_token		*t;

	parts = NULL;
	while (**pp && !ft_isspace((unsigned char)**pp) && !ft_strchr("|&<>()",
			**pp))
	{
		if (**pp == '\'' || **pp == '"')
			part = scan_quoted_part(pp);
		else if (**pp == '$')
			part = scan_param_part(pp);
		else
			part = scan_unquoted_part(pp);
		if (!part)
			return (free_parts(parts), NULL);
		append_part(&parts, part);
	}
	t = new_token(TOK_ARG, NULL);
	if (!t)
		return (free_parts(parts), NULL);
	t->u_u.arg.parts = parts;
	finalize_arg(&t->u_u.arg);
	return (t);
}

static t_token	*scan_symbol(const char **pp)
{
	const char	*p;
	t_token		*t;

	p = *pp;
	t = NULL;
	if (*p == '|')
		p = scan_pipe(p, &t);
	else if (*p == '&')
		p = scan_and(p, &t);
	else if (*p == '<')
		p = scan_redir_in(p, &t);
	else if (*p == '>')
		p = scan_redir_out(p, &t);
	else if (*p == '(' || *p == ')')
		p = scan_paren(p, &t);
	*pp = p;
	return (t);
}

t_token	*lexer(const char *input)
{
	const char	*p;
	t_token		*head;
	t_token		*tail;
	t_token		*tok;

	p = input;
	head = NULL;
	tail = NULL;
	while (*p)
	{
		while (*p && ft_isspace((unsigned char)*p))
			p++;
		if (!*p)
			break ;
		if (ft_strchr("|&<>()", *p))
			tok = scan_symbol(&p);
		else
			tok = scan_arg(&p);
		if (!tok)
			return (free_tokens(head), NULL);
		tok_push(&head, &tail, tok);
	}
	tok_push(&head, &tail, new_token(TOK_EOF, NULL));
	return (head);
}
