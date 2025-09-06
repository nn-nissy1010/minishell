/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_ptr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:47:41 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/07/30 12:11:11 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

static int	ptr_len(unsigned long long num)
{
	unsigned long long	len;

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

static int	put_pointer(unsigned long long num, int fd)
{
	char	c;
	int		result;

	if (num >= 16)
	{
		result = put_pointer(num / 16, fd);
		if (result < 0)
			return (-1);
	}
	if ((num % 16) <= 9)
		c = (num % 16) + '0';
	else
		c = (num % 16) - 10 + 'a';
	if (ft_put_char_fd(c, fd) < 0)
		return (-1);
	return (0);
}

int	ft_put_ptr_fd(void *ptr, int fd)
{
	int					count;
	unsigned long long	addr;

	addr = (unsigned long long)ptr;
	count = 0;
	if (!ptr)
		return (ft_put_str_fd("(nil)", fd));
	if (ft_put_str_fd("0x", fd) < 0)
		return (-1);
	if (put_pointer(addr, fd) < 0)
		return (-1);
	count = ptr_len(addr) + 2;
	return (count);
}
