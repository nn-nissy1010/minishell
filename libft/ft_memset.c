/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 15:51:46 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/07/30 12:08:47 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int ch, size_t n)
{
	size_t			i;
	unsigned char	*str;

	str = s;
	i = 0;
	while (i < n)
	{
		str[i] = ch;
		i++;
	}
	return (str);
}
