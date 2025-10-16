/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_bi_exit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:03:07 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/16 16:33:37 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	read_ws_and_sign(const char **ps, int *neg)
{
	const char	*s;

	s = *ps;
	*neg = 0;
	while (*s && isspace((unsigned char)*s))
		s++;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			*neg = 1;
		s++;
	}
	*ps = s;
}

static int	scan_u10_limited(const char **ps, unsigned long lim,
		unsigned long *acc)
{
	const char		*s;
	unsigned long	cut;
	int				d;
	int				any;

	s = *ps;
	*acc = 0;
	any = 0;
	if (!isdigit((unsigned char)*s))
		return (0);
	cut = lim / 10;
	while (isdigit((unsigned char)*s))
	{
		d = *s - '0';
		if (*acc > cut || (*acc == cut && (unsigned long)d > lim % 10))
			return (0);
		*acc = (*acc * 10) + (unsigned long)d;
		any = 1;
		s++;
	}
	*ps = s;
	return (any);
}

int	is_long(const char *s)
{
	const unsigned long	lpos = (unsigned long)LONG_MAX;
	const unsigned long	lneg = (unsigned long)LONG_MAX + 1UL;
	unsigned long		lim;
	unsigned long		acc;
	int					neg;

	if (!s)
		return (0);
	read_ws_and_sign(&s, &neg);
	if (neg)
		lim = lneg;
	else
		lim = lpos;
	if (!scan_u10_limited(&s, lim, &acc))
		return (0);
	while (*s && isspace((unsigned char)*s))
		s++;
	if (*s == '\0')
		return (1);
	return (0);
}

int	exit_validate(char *str)
{
	int	i;

	i = 0;
	if (is_long(str) == 0)
		return (0);
	if (str[i] == '-')
		i++;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int	bi_exit(char **av)
{
	int	code;

	code = get_exit_status();
	write(STDOUT_FILENO, "exit\n", 5);
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
