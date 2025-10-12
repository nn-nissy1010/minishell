/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_bi_exit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:03:07 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/12 12:29:04 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int exit_validate(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if(ft_isdigit(str[i]) == 0)
            return (0);
        i++;
    }
    return (1);
}

int    bi_exit(char **av)
{
    int    code;

    code = get_exit_status();
    write(STDOUT_FILENO, "exit\n", 5);
    if (av[1])
    {
        code = ft_atoi(av[1]);
        if(exit_validate(av[1]) == 0)
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
