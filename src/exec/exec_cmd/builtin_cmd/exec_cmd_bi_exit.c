/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_bi_exit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:03:07 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/04 23:37:43 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_exit(char **av)
{
	int	code;

	code = get_exit_status();
	write(STDOUT_FILENO, "exit\n", 5);
	if (av[1])
		code = ft_atoi(av[1]);
	set_exit_status(code & 0xFF);
	free_tokens(get_tokens());
    destroy_ast(get_ast());
	destroy_env_table();
	exit(get_exit_status());
}
