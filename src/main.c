/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:11:46 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/05 10:47:47 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void    update_shlvl(void)
{
    const char    *old;
    int        val;
    char    *buf;

    old = search_env_table("SHLVL");
    if (!old)
        val = 0;
    else
        val = atoi(old);

    val++;
    // bash 互換: SHLVL が異常に大きければリセット
    if (val >= 1000)
    {
        fprintf(stderr, "minishell: warning: shell level (%d) too high, resetting to 1\n", val);
        val = 1;
    }
	buf = ft_itoa(val);
    update_env_table("SHLVL", buf);
}


int	main(int argc, char **argv, char **envp)
{
	t_env_table	*table;

	(void)argc;
	(void)argv;
	
	table = env_table();
	if (env_table_init(table, 128) == -1)
		return (print_syntax_error("env alloc error"), 1);
	if (env_table_load_envp(table, envp) == -1)
		return (print_syntax_error("env load error"), destroy_env_table(table),
			1);
	if(search_env_table("TEST") != NULL)
		update_shlvl();
	else
		update_env_table("TEST", "test");
	repl();
	destroy_env_table(table);
	return (0);
}
