/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:17:28 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/07/30 12:08:41 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	const unsigned char	*src1;
	unsigned char		*dest1;

	dest1 = dest;
	src1 = src;
	i = 0;
	if (n == 0)
		return (dest);
	while (i < n)
	{
		dest1[i] = src1[i];
		i++;
	}
	return (dest);
}
