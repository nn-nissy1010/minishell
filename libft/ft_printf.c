/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:30:46 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/06/01 15:47:36 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

static int	ft_format(va_list *args, const char *format, int fd)
{
	if (*format == 'c')
		return (ft_put_char_fd(va_arg(*args, int), fd));
	else if (*format == 's')
		return (ft_put_str_fd(va_arg(*args, char *), fd));
	else if (*format == 'd' || *format == 'i')
		return (ft_put_int_fd(va_arg(*args, long), fd));
	else if (*format == 'u')
		return (ft_put_unsigned_fd(va_arg(*args, unsigned int), fd));
	else if (*format == 'p')
		return (ft_put_ptr_fd(va_arg(*args, void *), fd));
	else if (*format == 'X' || *format == 'x')
	{
		if (*format == 'X')
			return (ft_put_hex_fd(va_arg(*args, unsigned int), fd,
					HEX_UP_BASE));
		else
			return (ft_put_hex_fd(va_arg(*args, unsigned int), fd,
					HEX_LOW_BASE));
	}
	else if (*format == '%')
		return (ft_put_char_fd('%', fd));
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		count;
	int		tmp;

	count = 0;
	if (!format)
		return (-1);
	va_start(args, format);
	while (*format)
	{
		if ((*format == '%' && !*(format + 1)) || count > INT_MAX)
			return (va_end(args), -1);
		if (*format == '%' && *(format + 1))
			tmp = ft_format(&args, ++format, 1);
		else
			tmp = ft_put_char_fd(*format, 1);
		if (tmp < 0)
			return (va_end(args), -1);
		count += tmp;
		format++;
	}
	va_end(args);
	return (count);
}
