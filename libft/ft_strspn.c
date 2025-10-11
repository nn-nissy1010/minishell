/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strspn.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:30:03 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/07/30 12:11:27 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

size_t	ft_strspn(const char *str, const char *accept)
{
	size_t	i;
	size_t	j;
	size_t	check;

	i = 0;
	while (str[i] != '\0')
	{
		j = 0;
		check = 0;
		while (accept[j] != '\0')
		{
			if (str[i] == accept[j])
				check = 1;
			j++;
		}
		if (check == 0)
			return (i);
		i++;
	}
	return (i);
}
