/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:11:46 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/25 23:45:55 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- 最小ヘルパ --- */
static t_token	*new_arg_token_simple(const char *s)
{
	t_token		*t;
	t_arg_part	*p;
	size_t		len;

	t = (t_token *)calloc(1, sizeof(*t));
	if (!t)
		return (NULL);
	p = (t_arg_part *)calloc(1, sizeof(*p));
	if (!p)
	{
		free(t);
		return (NULL);
	}
	t->type = TOK_ARG;
	t->u.arg.raw = strdup(s ? s : "");
	if (!t->u.arg.raw)
	{
		free(p);
		free(t);
		return (NULL);
	}
	len = strlen(s ? s : "");
	p->text = (char *)malloc(len + 1);
	if (!p->text)
	{
		free(t->u.arg.raw);
		free(p);
		free(t);
		return (NULL);
	}
	memcpy(p->text, s ? s : "", len + 1);
	p->quote = Q_NONE;
	p->has_param = (strchr(p->text, '$') != NULL);
	p->has_unq_glob = 0;
	p->next = NULL;
	t->u.arg.parts = p;
	return (t);
}

static void	free_token_deep(t_token *t)
{
	size_t	i;

	t_arg_part *p, *n;
	if (!t)
		return ;
	if (t->type == TOK_ARG)
	{
		p = t->u.arg.parts;
		while (p)
		{
			n = p->next;
			free(p->text);
			free(p);
			p = n;
		}
		free(t->u.arg.raw);
		if (t->u.arg.items)
		{
			i = 0;
			while (t->u.arg.items[i])
				free(t->u.arg.items[i++]);
			free(t->u.arg.items);
		}
	}
	free(t);
}

static void	free_cmd_tokens(t_cmd *c)
{
	size_t	i;

	if (!c || !c->argv_tokens)
		return ;
	i = 0;
	while (i < c->n_argv_tokens)
	{
		free_token_deep(c->argv_tokens[i]);
		i++;
	}
	free(c->argv_tokens);
	c->argv_tokens = NULL;
	c->n_argv_tokens = 0;
}

static void	free_argv(t_cmd *c)
{
	size_t	i;

	if (!c || !c->argv)
		return ;
	i = 0;
	while (c->argv[i])
	{
		free(c->argv[i]);
		i++;
	}
	free(c->argv);
	c->argv = NULL;
	c->argc = 0;
}

static void	dump_argv(const t_cmd *c)
{
	size_t	i;

	printf("argc=%zu\n", c->argc);
	i = 0;
	while (i < c->argc)
	{
		printf("argv[%zu]=\"%s\"\n", i, (c->argv
				&& c->argv[i]) ? c->argv[i] : "(null)");
		i++;
	}
}

/* --- Case1: echo $HOME ~（正常系：3トークンのみ） --- */
static void	run_case1(void)
{
	t_cmd	c;
	int		rc;

	memset(&c, 0, sizeof(c));
	env_table_set(env_table(), "HOME", "/home/testuser");
	c.n_argv_tokens = 3;
	c.argv_tokens = (t_token **)calloc(c.n_argv_tokens, sizeof(*c.argv_tokens));
	if (!c.argv_tokens)
		return ;
	c.argv_tokens[0] = new_arg_token_simple("echo");
	c.argv_tokens[1] = new_arg_token_simple("$HOME");
	c.argv_tokens[2] = new_arg_token_simple("~");
	reset_exit_status();
	printf("== Case1: echo $HOME ~ ==\n");
	rc = expansion(&c);
	printf("rc=%d\n", rc);
	printf("exit_status=%d\n", get_exit_status());
	dump_argv(&c);
	printf("\n");
	free_argv(&c);
	free_cmd_tokens(&c);
}

/* --- Case2: redir word に $FOO（"aaa bbb"）→ 分裂で失敗 --- */
static void	run_case2(void)
{
	t_cmd	c;
	t_redir	r;
	t_token	*w;
	int		rc;

	memset(&c, 0, sizeof(c));
	memset(&r, 0, sizeof(r));
	env_table_set(env_table(), "FOO", "aaa bbb");
	w = new_arg_token_simple("$FOO"); /* これだけで分裂トリガに十分 */
	r.word = w;
	c.redirs = &r;
	c.n_redirs = 1;
	reset_exit_status();
	printf("== Case2: redir word = $FOO (\"aaa bbb\") -> expect failure ==\n");
	rc = expansion(&c);
	printf("rc=%d\n", rc);
	printf("exit_status=%d\n", get_exit_status());
	printf("redir.path=\"%s\"\n", r.path ? r.path : "(null)");
	printf("argc=%zu\n\n", c.argc);
	if (r.path)
		free(r.path);
	free_token_deep(w);
	free_argv(&c);
	free_cmd_tokens(&c);
}

int	main(void)
{
	run_case1();
	run_case2();
	return (0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
//     (void)argv;
// 	t_env_table *table;
// 	table = env_table();
// 	if (env_table_init(table, 128) == -1)
// 		return(print_syntax_error("env alloc error"), 1);
// 	if (env_table_load_envp(table, envp) == -1)
// 		return(print_syntax_error("env load error"), destroy_env_table(table),
//		1);
// 	repl();
// 	return (0);
// }
