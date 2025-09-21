
// #include "../../includes/minishell.h"

#include "./../../includes/exec.h"
#include <stdio.h>
#include <stdlib.h>

/* ------- テスト用ヘルパ ------- */

static t_token	*mk_arg(const char *s)
{
	t_token	*t;

	t = (t_token *)calloc(1, sizeof *t);
	if (!t)
		return (NULL);
	t->type = TOK_ARG;
	t->u.arg.raw = s ? strdup(s) : strdup("");
	/* 他フィールドは 0/NULL で十分（初期値） */
	if (!t->u.arg.raw)
	{
		free(t);
		return (NULL);
	}
	return (t);
}

static void	push_arg(t_token **head, t_token **tail, const char *s)
{
	t_token	*n;

	n = mk_arg(s);
	if (!n)
	{
		fprintf(stderr, "alloc failed in push_arg\n");
		exit(1);
	}
	if (!*head)
		*head = n;
	else
		(*tail)->next = n;
	*tail = n;
}

static void	free_argv(char **argv)
{
	if (!argv)
		return ;
	for (size_t i = 0; argv[i]; i++)
		free(argv[i]);
	free(argv);
}

static void	print_argv(const char *label, char **argv)
{
	size_t	i;

	printf("== %s ==\n", label);
	if (!argv)
	{
		puts("(null)");
		return ;
	}
	i = 0;
	while (argv[i])
	{
		printf("argv[%zu] = \"%s\"\n", i, argv[i]);
		i++;
	}
	printf("argc = %zu\n\n", i);
}

/* ------- メインテスト ------- */
int	main(void)
{
	/* テスト環境差し込み */
	setenv("HOME", "/tmp/TESTHOME", 1); // ここを変えれば出力も変わる

	int rc;
	char **argv;

	// Case1: echo "$HOME aaa"  → 期待: ["echo", "/tmp/TESTHOME aaa"]
	t_token *h1 = NULL, *t1 = NULL;
	push_arg(&h1, &t1, "echo");
	push_arg(&h1, &t1, "\"$HOME aaa\"");
	rc = build_argv_from_tokens(h1, &argv);
	printf("Case1 rc=%d\n", rc);
	print_argv("echo \"$HOME aaa\"", argv);
	free_argv(argv);
	// free_tokens(h1);

	// Case2: echo '$HOME aaa'  → 単引用内は展開しない: ["echo", "$HOME aaa"]
	t_token *h2 = NULL, *t2 = NULL;
	push_arg(&h2, &t2, "echo");
	push_arg(&h2, &t2, "'$HOME aaa'");
	rc = build_argv_from_tokens(h2, &argv);
	printf("Case2 rc=%d\n", rc);
	print_argv("echo '$HOME aaa'", argv);
	free_argv(argv);
	// free_tokens(h2);

	// Case3: $HOME 単体 → ["/tmp/TESTHOME"]
	t_token *h3 = NULL, *t3 = NULL;
	push_arg(&h3, &t3, "$HOME");
	rc = build_argv_from_tokens(h3, &argv);
	printf("Case3 rc=%d\n", rc);
	print_argv("$HOME", argv);
	free_argv(argv);
	// free_tokens(h3);

	// Case4: 未定義 VAR → 空文字（1語）※現仕様
	unsetenv("FOO");
	t_token *h4 = NULL, *t4 = NULL;
	push_arg(&h4, &t4, "$FOO");
	rc = build_argv_from_tokens(h4, &argv);
	printf("Case4 rc=%d\n", rc);
	print_argv("$FOO (unset)", argv);
	free_argv(argv);
	// free_tokens(h4);

	return (0);
}