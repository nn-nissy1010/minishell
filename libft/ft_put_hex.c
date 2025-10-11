/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_hex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:40:22 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/07/30 12:11:20 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

static int	hex_len(unsigned int num)
{
	unsigned int	len;

	if (num == 0)
		return (1);
	len = 0;
	while (num > 0)
	{
		len++;
		num = num / 16;
	}
	return (len);
}

int	ft_put_hex_fd(unsigned int num, int fd, const char *base)
{
	char	buffer[16];
	int		len;
	int		i;

	if (num == 0)
	{
		buffer[0] = '0';
		buffer[1] = '\0';
		return (ft_put_str_fd(buffer, fd));
	}
	len = hex_len(num);
	i = len;
	buffer[i] = '\0';
	while (num > 0)
	{
		buffer[--i] = base[num % 16];
		num = num / 16;
	}
	return (ft_put_str_fd(buffer, fd));
}
