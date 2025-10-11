/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:15:59 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/07/30 12:07:45 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*result;

	if (size != 0 && nmemb > __SIZE_MAX__ / size)
	{
		return (NULL);
	}
	if (nmemb == 0 || size == 0)
	{
		return (malloc(0));
	}
	result = (void *)malloc(nmemb * size);
	if (!result)
	{
		return (NULL);
	}
	ft_memset(result, '\0', (nmemb * size));
	return (result);
}
