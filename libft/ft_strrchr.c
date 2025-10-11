/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:49:41 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/07/30 12:10:09 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*str;
	char	*tmp;

	str = (char *)s;
	tmp = NULL;
	while (*str != '\0')
	{
		if (*str == (char)c)
		{
			tmp = str;
		}
		str++;
	}
	if (*str == (char)c)
	{
		return (str);
	}
	return (tmp);
}
