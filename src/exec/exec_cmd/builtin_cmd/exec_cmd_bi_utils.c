/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_bi_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:04:00 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/13 14:00:49 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_line(int fd, const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = ft_strlen(s);
	if (write(fd, s, len) < 0)
		return (-1);
	if (write(fd, "\n", 1) < 0)
		return (-1);
	return (0);
}

int	is_valid_ident(const char *s)
{
	unsigned char	c;

	if (!s || !*s)
		return (0);
	c = (unsigned char)*s;
	if (!(ft_isalpha(c) || c == '_'))
		return (0);
	s++;
	while (*s)
	{
		c = (unsigned char)*s;
		if (*s == '=')
			return (0);
		if (!(ft_isalnum(c) || c == '_'))
			return (0);
		s++;
	}
	return (1);
}

static void	print_export_line(const char *key, const char *val)
{
	write(STDOUT_FILENO, "declare -x ", 11);
	write(STDOUT_FILENO, key, ft_strlen(key));
	if (val)
	{
		write(STDOUT_FILENO, "=\"", 2);
		write(STDOUT_FILENO, val, ft_strlen(val));
		write(STDOUT_FILENO, "\"", 1);
	}
	write(STDOUT_FILENO, "\n", 1);
}

void	bi_export_display(void)
{
	size_t		i;
	t_env_pair	*pair;
	t_env_table	*table;

	table = env_table();
	if (!table || !table->pair)
		return ;
	i = 0;
	while (i < table->cap)
	{
		pair = &table->pair[i];
		if (pair->key && !pair->tomb && ft_strcmp(pair->key, "_") != 0)
			print_export_line(pair->key, pair->val);
		i++;
	}
}
