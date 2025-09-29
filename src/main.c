/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:11:46 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/29 16:31:43 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ===== readline スタブ =====
   ライブラリより前にこのオブジェクトをリンクして、確実にこちらを使わせる */
static const char **g_lines = NULL;
static size_t g_nlines = 0;
static size_t g_idx = 0;

char *readline(const char *prompt)
{
	size_t len;
	char *s;

	(void)prompt;
	if (g_lines == NULL)
		return NULL;
	if (g_idx >= g_nlines)
		return NULL;
	len = strlen(g_lines[g_idx]);
	s = (char *)malloc(len + 1);
	if (s == NULL)
		return NULL;
	memcpy(s, g_lines[g_idx], len + 1);
	g_idx++;
	return s;
}

/* ===== 最小限のトークン生成 ===== */
static t_arg_part *new_part(const char *text, t_quote_type q)
{
	t_arg_part *p;

	p = (t_arg_part *)calloc(1, sizeof(*p));
	if (p == NULL)
		return NULL;
	if (text != NULL)
		p->text = strdup(text);
	else
		p->text = strdup("");
	if (p->text == NULL)
	{
		free(p);
		return NULL;
	}
	p->quote = q;
	p->has_param = 0;
	p->has_unq_glob = 0;
	p->next = NULL;
	return p;
}

static t_token *new_arg_token_one_part(const char *word, t_quote_type q)
{
	t_token *t;

	t = (t_token *)calloc(1, sizeof(*t));
	if (t == NULL)
		return NULL;
	t->type = TOK_ARG;
	if (word != NULL)
		t->u.arg.raw = strdup(word);
	else
		t->u.arg.raw = strdup("");
	if (t->u.arg.raw == NULL)
	{
		free(t);
		return NULL;
	}
	t->u.arg.parts = new_part(word, q);
	if (t->u.arg.parts == NULL)
	{
		free(t->u.arg.raw);
		free(t);
		return NULL;
	}
	t->u.arg.items = NULL;
	t->u.arg.n_items = 0;
	t->u.arg.expanded = 0;
	t->next = NULL;
	return t;
}

static void free_arg_token(t_token *t)
{
	size_t i;

	if (t == NULL)
		return;
	if (t->type == TOK_ARG)
	{
		free_parts(t->u.arg.parts); /* プロジェクト側の実装 */
		if (t->u.arg.items != NULL)
		{
			i = 0;
			while (t->u.arg.items[i] != NULL)
			{
				free(t->u.arg.items[i]);
				i++;
			}
			free(t->u.arg.items);
		}
		free(t->u.arg.raw);
	}
	free(t);
}

/* ===== redir / cmd 構築 ===== */
static void redir_make_heredoc(t_redir *r, t_token *delim_tok, t_quote_type q)
{
	r->fd = -1;
	r->kind = TOK_HEREDOC;
	r->word = delim_tok;
	if (q == Q_NONE)
		r->quoted_heredoc = 0;
	else
		r->quoted_heredoc = 1;
	r->path = NULL;
}

static void cmd_init_1_heredoc(t_cmd *c, t_token *delim_tok, t_quote_type q)
{
	c->redirs = (t_redir *)calloc(1, sizeof(t_redir));
	c->n_redirs = 1;
	redir_make_heredoc(&c->redirs[0], delim_tok, q);
	c->argv_tokens = NULL;
	c->n_argv_tokens = 0;
	c->argv = NULL;
	c->argc = 0;
}

static void cmd_destroy(t_cmd *c)
{
	size_t i;
	size_t k;

	if (c == NULL)
		return;
	i = 0;
	while (i < c->n_redirs)
	{
		if (c->redirs[i].fd >= 0)
			close(c->redirs[i].fd);
		free_arg_token(c->redirs[i].word);
		free(c->redirs[i].path);
		i++;
	}
	free(c->redirs);
	if (c->argv != NULL)
	{
		k = 0;
		while (c->argv[k] != NULL)
		{
			free(c->argv[k]);
			k++;
		}
		free(c->argv);
	}
}

/* ===== fd 全読み（パイプ/ファイル両対応） ===== */
static char *read_all_from_fd(int fd)
{
	size_t cap;
	size_t len;
	char *buf;
	size_t ncap;
	char *nb;
	ssize_t n;

	cap = 1024;
	len = 0;
	buf = (char *)malloc(cap);
	if (buf == NULL)
		return NULL;
	for (;;)
	{
		if (len + 512 > cap)
		{
			ncap = cap * 2;
			nb = (char *)realloc(buf, ncap);
			if (nb == NULL)
			{
				free(buf);
				return NULL;
			}
			buf = nb;
			cap = ncap;
		}
		n = read(fd, buf + len, 512);
		if (n < 0)
		{
			free(buf);
			return NULL;
		}
		if (n == 0)
			break;
		len += (size_t)n;
	}
	if (len + 1 > cap)
	{
		nb = (char *)realloc(buf, len + 1);
		if (nb == NULL)
		{
			free(buf);
			return NULL;
		}
		buf = nb;
	}
	buf[len] = '\0';
	return buf;
}

/* ===== 1ケース実行 ===== */
static void run_case(const char *title, const char *delim, t_quote_type q,
                     const char **lines, size_t nlines)
{
	t_cmd c;
	t_token *delim_tok;
	int rc;
	char *got;
	int fd;

	printf("== %s ==\n", title);

	/* 入力（readlineスタブ用） */
	g_lines = lines;
	g_nlines = nlines;
	g_idx = 0;

	delim_tok = new_arg_token_one_part(delim, q);
	if (delim_tok == NULL)
	{
		printf("alloc error\n\n");
		return;
	}
	memset(&c, 0, sizeof(c));
	cmd_init_1_heredoc(&c, delim_tok, q);

	rc = collect_heredocs(&c);
	printf("rc=%d\n", rc);
	if (rc == 0)
	{
		fd = c.redirs[0].fd;
		printf("fd=%d (>=0ならOK)\n", fd);
		if (fd >= 0)
		{
			/* 一時ファイル実装に備えて先頭へ、パイプなら ESPIPE→無視 */
			(void)lseek(fd, 0, SEEK_SET);

			got = read_all_from_fd(fd);
			if (got != NULL)
			{
				printf("content:\n---\n%s---\n", got);
				free(got);
			}
			else
			{
				printf("content: (read error)\n");
			}
		}
	}
	else
	{
		printf("collect_heredocs failed\n");
	}

	cmd_destroy(&c);
	printf("\n");
}

int main(void)
{
	/* Case1: 非引用: 展開あり実装なら展開対象 */
	const char *lines1[] = { "hello", "world", "EOF" };

	/* Case2: 単一引用: 展開しない実装が多い */
	const char *lines2[] = { "$USER", "EOF" };


	run_case("heredoc: unquoted delimiter (<< EOF)",
	         "EOF", Q_NONE,
	         lines1, sizeof(lines1) / sizeof(lines1[0]));

	run_case("heredoc: single-quoted delimiter (<< 'EOF')",
	         "EOF", Q_SINGLE,
	         lines2, sizeof(lines2) / sizeof(lines2[0]));

	return 0;
}



// int	main(int argc, char **argv, char **envp)
// {
// 	t_env_table	*table;

// 	(void)argc;
// 	(void)argv;
// 	table = env_table();
// 	if (env_table_init(table, 128) == -1)
// 		return (print_syntax_error("env alloc error"), 1);
// 	if (env_table_load_envp(table, envp) == -1)
// 		return (print_syntax_error("env load error"), destroy_env_table(table),
// 			1);
// 	repl();
// 	destroy_env_table(table);
// 	return (0);
// }
