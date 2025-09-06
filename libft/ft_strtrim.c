/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:58:55 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/07/30 12:10:20 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	char	*str;
	size_t	setstart;
	size_t	setend;

	if (!s1 || !set)
		return (NULL);
	setstart = 0;
	while (s1[setstart] && ft_strchr(set, s1[setstart]))
	{
		setstart++;
	}
	setend = ft_strlen(s1);
	while (setend > setstart && ft_strchr(set, s1[setend - 1]))
		setend--;
	str = malloc(sizeof(char) * (setend - setstart + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (setstart < setend)
		str[i++] = s1[setstart++];
	str[i] = '\0';
	return (str);
}
