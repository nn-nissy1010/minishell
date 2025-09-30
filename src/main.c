/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:11:46 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/30 20:38:09 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

/* ---- テスト用ファイル/ディレクトリ作成 ---- */
static void touch(const char *path)
{
	int fd = open(path, O_CREAT | O_WRONLY, 0644);
	if (fd >= 0)
		close(fd);
}

static int setup_fs(void)
{
	if (mkdir("_globtest", 0755) < 0 && access("_globtest", F_OK) != 0)
		return -1;

	touch("_globtest/a.c");
	touch("_globtest/ab.c");
	touch("_globtest/aaa.txt");
	touch("_globtest/abc.txt");
	touch("_globtest/b.txt");
	touch("_globtest/note.md");
	touch("_globtest/Makefile");
	(void)mkdir("_globtest/dir", 0755);
	touch("_globtest/dir/x.c");
	return 0;
}


static void print_argv(const t_cmd *c)
{
	size_t i = 0;
	printf("argc=%zu\n", c->argc);
	while (i < (size_t)c->argc && c->argv && c->argv[i])
	{
		printf("argv[%zu]=\"%s\"\n", i, c->argv[i]);
		i++;
	}
}

static char **dup_argv_heap(const char *const *src, int argc)
{
	int i = 0;
	char **v = (char **)malloc(sizeof(char *) * (argc + 1));
	if (!v)
		return NULL;
	while (i < argc)
	{
		size_t len = strlen(src[i]);
		v[i] = (char *)malloc(len + 1);
		if (!v[i])
		{
			int k = 0;
			while (k < i)
			{
				free(v[k]);
				k++;
			}
			free(v);
			return NULL;
		}
		memcpy(v[i], src[i], len + 1);
		i++;
	}
	v[argc] = NULL;
	return v;
}


static void cmd_init(t_cmd *c, char **argv, int argc)
{
	c->argv = argv;
	c->argc = argc;
	c->redirs = NULL;
	c->n_redirs = 0;
}


static void run_case(const char *title, const char *const *argv_lit, int argc_in)
{
	t_cmd c;
	int rc;
	char **argv_heap = dup_argv_heap(argv_lit, argc_in);

	printf("\n== %s ==\n", title);
	if (!argv_heap)
	{
		perror("dup_argv_heap");
		return;
	}
	cmd_init(&c, argv_heap, argc_in);

	rc = glob_expand_argv(&c);
	printf("rc=%d\n", rc);
	print_argv(&c);

}

int main(void)
{
	char cwd[1024];

	if (setup_fs() != 0)
	{
		perror("setup_fs");
		return 1;
	}
	if (!getcwd(cwd, sizeof(cwd)))
		return 1;
	if (chdir("_globtest") != 0)
	{
		perror("chdir");
		return 1;
	}

	/* Case 1: 基本（*.txt が複数に展開） */
	const char *case1[] = { "echo", "*.txt", NULL };
	run_case("basic: *.txt expands to multiple", case1, 2);

	/* Case 2: 混在（リテラル + グロブ + リテラル） */
	const char *case2[] = { "cat", "a*.c", "Makefile", NULL };
	run_case("mixed: a*.c expands, others stay", case2, 3);

	/* Case 3: マッチ0件（そのまま残す想定） */
	const char *case3[] = { "echo", "zzz*.dat", NULL };
	run_case("no match: keep literal", case3, 2);

	/* Case 4: スラッシュ含み → 展開しない（should_glob_expand が弾く想定） */
	const char *case4[] = { "echo", "dir/*", NULL };
	run_case("has slash: do not expand", case4, 2);

	/* Case 5: 複数スター（a*.*） */
	const char *case5[] = { "echo", "a*.*", NULL };
	run_case("multi-star pattern", case5, 2);

	/* Case 6: /入り（./a*.*） */
	const char *case6[] = { "echo", "./a*.*", NULL };
	run_case("multi-star pattern", case6, 2);

	/* 戻す（_globtest はそのまま残す） */
	(void)chdir(cwd);
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
