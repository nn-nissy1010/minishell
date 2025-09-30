/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 01:44:26 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/30 10:18:24 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int run_builtin_parent(t_cmd *c)
{
    char **av;

    if (!c || !c->argv || !c->argv[0])
        return 0;
    av = c->argv;
    if (ft_strcmp(av[0], "cd") == 0)
        return bi_cd(av);          
    if (ft_strcmp(av[0], "export") == 0)
        return bi_export(av);     
    if (ft_strcmp(av[0], "unset") == 0)
        return bi_unset(av);       
    if (ft_strcmp(av[0], "exit") == 0)
        return bi_exit(av);        
    return -1;
}

