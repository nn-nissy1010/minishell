/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 22:13:36 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/23 22:21:30 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 最小ヘルパ */
t_arg_part	*part_new(const char *txt, t_quote_type q, int has_param)
{
	t_arg_part	*p;

	p = (t_arg_part *)malloc(sizeof(*p));
	if (!p)
		return (NULL);
	p->text = ft_strdup(txt ? txt : "");
	p->quote = q;
	p->has_param = has_param;
	p->has_unq_glob = 0;
	p->next = NULL;
	return (p);
}

t_arg_part	*chain(t_arg_part *a, t_arg_part *b)
{
	if (a)
		a->next = b;
	return (a ? a : b);
}

t_token	*tok_from_parts(t_arg_part *parts)
{
	t_token	*t;

	t = (t_token *)malloc(sizeof(*t));
	if (!t)
		return (NULL);
	t->type = TOK_ARG;
	t->u.arg.raw = NULL;
	t->u.arg.parts = parts;
	t->u.arg.items = NULL;
	t->u.arg.n_items = 0;
	t->u.arg.expanded = 0;
	t->next = NULL;
	return (t);
}

void	parts_free(t_arg_part *p)
{
	t_arg_part	*n;

	while (p)
	{
		n = p->next;
		free(p->text);
		free(p);
		p = n;
	}
}

void	token_free_deep(t_token *t)
{
	if (!t)
		return ;
	if (t->type == TOK_ARG)
		parts_free(t->u.arg.parts);
	free(t);
}

void	redir_make(t_redir *r, t_token *word_tok, int fd)
{
	r->fd = fd;
	r->word = word_tok;
	r->quoted_heredoc = 0;
	r->path = NULL;
}

/* --- 1ケース実行（連結・展開結果だけ確認） --- */
void	run_case(const char *title, t_token *word_tok)
{
	t_redir r;
	int rc;

	redir_make(&r, word_tok, -1);
	rc = expand_redirs(&r, 1);
	printf("== %s\n", title);
	printf("   rc=%d\n", rc);
	printf("   path=\"%s\"\n", r.path ? r.path : "(null)");
	if (r.path)
		free(r.path);
	token_free_deep(word_tok);
}