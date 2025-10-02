/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_bi_echo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 01:44:26 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/02 00:26:21 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static int	skip_n_flags(char **av, int *pi)
{
	int	i;
	int	nflag;
	int	j;

	i = 1;
	nflag = 0;
	while (av[i] && av[i][0] == '-' && av[i][1] == 'n')
	{
		j = 1;
		while (av[i][j] == 'n')
			j++;
		if (av[i][j] != '\0')
			break ;
		nflag = 1;
		i++;
	}
	*pi = i;
	return (nflag);
}

int	bi_echo(char **av)
{
	int	i;
	int	nflag;

	if (!av)
		return (0);
	nflag = skip_n_flags(av, &i);
	while (av[i])
	{
		write(STDOUT_FILENO, av[i], ft_strlen(av[i]));
		if (av[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (!nflag)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
