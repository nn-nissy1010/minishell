/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_alnum.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:38:29 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/07/30 12:10:57 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

int	ft_put_char_fd(char c, int fd)
{
	int	error;

	error = write(fd, &c, 1);
	if (error < 0)
		return (-1);
	return (1);
}

int	ft_put_str_fd(char *str, int fd)
{
	size_t	i;
	size_t	count;
	int		error;

	i = 0;
	count = 0;
	error = 0;
	if (!str)
	{
		write(fd, "(null)", 6);
		return (6);
	}
	while (str[i])
	{
		error = write(fd, &str[i], 1);
		if (error < 0)
			return (-1);
		i++;
		count++;
	}
	return (count);
}

int	ft_put_int_fd(int nbr, int fd)
{
	int		len;
	char	*num;

	len = 0;
	num = ft_itoa(nbr);
	if (!num)
		return (-1);
	len = ft_put_str_fd(num, fd);
	free(num);
	return (len);
}

int	ft_put_unsigned_fd(unsigned int nbr, int fd)
{
	int		len;
	char	*num;

	len = 0;
	num = ft_uitoa(nbr);
	if (!num)
		return (-1);
	len = ft_put_str_fd(num, fd);
	free(num);
	return (len);
}
