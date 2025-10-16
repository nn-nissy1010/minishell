/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_child_bi_exit.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:03:07 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/17 00:50:39 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	child_bi_exit(char **av)
{
	int	code;

	code = get_exit_status();
	if (av[1])
	{
		code = ft_atoi(av[1]);
		if (exit_validate(av[1]) == 0)
		{
			write(STDOUT_FILENO, "bash: exit: ", 12);
			write(STDOUT_FILENO, av[1], ft_strlen(av[1]));
			write(STDOUT_FILENO, ": 数字の引数が必要です\n", 33);
			code = 2;
		}
	}
	set_exit_status(code & 0xFF);
	free_tokens(get_tokens());
	destroy_ast(get_ast());
	destroy_env_table();
	exit(get_exit_status());
}
