/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:11:46 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/23 22:07:54 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int	main(void)
{
	t_env_table	*T;

	T = env_table();
	if (env_table_init(T, 128) == -1)
		return (1);
	/* 環境テーブルに投入 */
	env_table_set(T, "HOME", "/home/tester");
	env_table_set(T, "FOO", "a b");
	env_table_set(T, "EMPTY", "");
	// テスト開始
	printf("=== expand_redirs: minimal (join-only) tests ===\n");
	//
	/* 1)"$FOO"→ "a b" */
	run_case("> \"$FOO\"", tok_from_parts(part_new("$FOO", Q_DOUBLE, 1)));
	//
	/* 2)$FOO → "a b" */
	run_case("> $FOO", tok_from_parts(part_new("$FOO", Q_NONE, 1)));
	//
	/* 3) $EMPTY  */
	run_case("> $EMPTY", tok_from_parts(part_new("$EMPTY", Q_NONE, 1)));
	//
	/* 4) ~/x.txt */
	run_case("> ~/x.txt", tok_from_parts(part_new("~/x.txt", Q_NONE, 0)));
	//
	/* 5) x~  */
	run_case("> x~", tok_from_parts(part_new("x~", Q_NONE, 0)));
	//
	/* 6) "$HOME/$FOO"  → "/home/tester/a b" */
	run_case("> \"$HOME/$FOO\"", tok_from_parts(chain(part_new("$HOME/",
					Q_DOUBLE, 1), part_new("$FOO", Q_DOUBLE, 1))));
	//
	printf("=== done ===\n");
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
