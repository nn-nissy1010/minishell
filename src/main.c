/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:11:46 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/19 16:12:03 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
    (void)argv;
	t_env_table *table;
	table = env_table();
	if (env_table_init(table, 128) == -1)
		return(print_syntax_error("env alloc error"), 1); 
	if (env_table_load_envp(table, envp) == -1)
		return(print_syntax_error("env load error"), destroy_env_table(table), 1);
	repl();
	return (0);
}
