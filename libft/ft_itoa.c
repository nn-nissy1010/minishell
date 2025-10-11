/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 12:39:03 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/07/30 12:08:02 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_intlen(long n)
{
	size_t	len;

	len = 0;
	if (n <= 0)
	{
		len++;
		n = -n;
	}
	while (n > 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	size_t	len;
	char	*result;
	long	nb;

	nb = (long)n;
	if (nb == INT_MIN)
		return (ft_strdup("-2147483648"));
	len = ft_intlen(nb);
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	result[len--] = '\0';
	if (nb == 0)
		result[0] = '0';
	else if (nb < 0)
	{
		result[0] = '-';
		nb = -nb;
	}
	while (nb > 0)
	{
		result[len--] = (nb % 10) + '0';
		nb = nb / 10;
	}
	return (result);
}
