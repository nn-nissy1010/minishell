/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_bi_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:04:00 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/02 19:38:40 by tkuwahat         ###   ########.fr       */
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
