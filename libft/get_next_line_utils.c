/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 16:50:44 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/08/07 19:44:10 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

char	*gel_ft_strdup(const char *str)
{
	size_t	i;
	char	*dest;
	size_t	len;

	if (!str)
		return (NULL);
	i = 0;
	len = ft_strlen(str);
	dest = malloc(len + 1);
	if (!dest)
		return (NULL);
	while (str[i] != '\0')
	{
		dest[i] = str[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*substr;
	size_t	s_len;
	size_t	sub_size;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if ((s_len - start) > len)
		sub_size = len;
	else
		sub_size = s_len - start;
	substr = malloc(sizeof(char) * (sub_size + 1));
	if (!substr)
		return (NULL);
	i = 0;
	while (i < sub_size)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

char	*gnl_ft_strjoin(const char *str1, const char *str2)
{
	size_t	all_len;
	size_t	i;
	size_t	j;
	char	*dest;

	if (!str1 && !str2)
		return (NULL);
	if (!str1)
		return (ft_strdup(str2));
	if (!str2)
		return (ft_strdup(str1));
	all_len = ft_strlen(str1) + ft_strlen(str2);
	dest = malloc(sizeof(char) * (all_len + 1));
	if (!dest)
		return (NULL);
	i = -1;
	while (str1[++i] != '\0')
		dest[i] = str1[i];
	j = -1;
	while (str2[++j] != '\0')
		dest[i + j] = str2[j];
	dest[all_len] = '\0';
	return (dest);
}

char	*gel_ft_strchr(const char *s, int c)
{
	char	*str;

	str = (char *)s;
	while (*str != '\0')
	{
		if (*str == (char)c)
			return (str);
		str++;
	}
	if (*str == (char)c)
		return (str);
	return (NULL);
}
