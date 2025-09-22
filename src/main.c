/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:11:46 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/22 18:55:14 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int	main(void)
{
	t_exec_ctx	ctx;
	t_node		*n;
	int			rc;

	ctx.xflag = XF_NONE;
	printf("=== exec_cmd UNIT TEST ===\n");
	n = make_single_cmd_node("echo", "hi", NULL);
	rc = ast_exec(n, &ctx);
	printf("[ret ] exec_cmd rc=%d \n", rc);
	ast_destroy(n);
	free(n);
	printf("=== exec_cmd UNIT TEST ===\n");
	n = make_single_cmd_node("echo", "a$HOME", "b'*c*'", "\"$PATH\"", NULL);
	rc = ast_exec(n, &ctx);
	printf("[ret ] exec_cmd rc=%d \n", rc);
	ast_destroy(n);
	free(n);
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
