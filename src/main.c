/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:11:46 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/18 11:21:01 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_table g_env;

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
    (void)argv;
	if (env_table_init(&g_env, 128) == -1)
		return(print_syntax_error("env alloc error"), 1); 
	if (env_table_load_envp(&g_env, envp) == -1)
		return(print_syntax_error("env load error"), destroy_env_table(&g_env), 1);
	
	// printf("=== 初期 env ===\n");
    // get_env_table();

	// printf("\n=== 検索テスト ===\n");
    // const char *val;

    // val = search_env_table("PATH");
    // if (val)
    //     printf("PATH = %s\n", val);
    // else
    //     printf("PATH が見つかりません\n");

    // val = search_env_table("HOME");
    // if (val)
    //     printf("HOME = %s\n", val);
    // else
    //     printf("HOME が見つかりません\n");

    // val = search_env_table("NOT_EXIST");
    // if (val)
    //     printf("NOT_EXIST = %s\n", val);
    // else
    //     printf("NOT_EXIST が見つかりません\n");


    // printf("\n=== export TEST=hello ===\n");
    // update_env_table("TEST=hello");
    // get_env_table();

    // printf("\n=== export PATH=overwritten ===\n");
    // update_env_table("PATH=overwritten");
    // get_env_table();

    // printf("\n=== unset TEST ===\n");
    // unset_env_table("TEST");
    // get_env_table();
    // return 0;

	repl();
	return (0);
}
