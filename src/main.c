/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:11:46 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/02 01:38:20 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	repl();
	destroy_env_table(table);
	return (0);
}
