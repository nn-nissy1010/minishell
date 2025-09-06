/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:06:07 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/07/30 12:09:11 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	word_count(char *str, char delimiter)
{
	int	i;
	int	in_word;
	int	count;

	i = 0;
	in_word = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] != delimiter && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (str[i] == delimiter)
			in_word = 0;
		i++;
	}
	return (count);
}

static char	*mini_word(char *str, int len)
{
	int		i;
	char	*word;

	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static void	free_result(char **result, int count)
{
	int	i;

	i = 0;
	while (i < count && result[i])
	{
		free(result[i]);
		i++;
	}
	free(result);
}

static int	export_word(char *str, char **result, char delimiter)
{
	int	len;
	int	i;

	i = 0;
	while (*str)
	{
		while (*str == delimiter)
			str++;
		len = 0;
		while (str[len] && str[len] != delimiter)
			len++;
		if (len > 0)
		{
			result[i] = mini_word(str, len);
			if (!result[i])
			{
				free_result(result, i);
				return (0);
			}
			str += len;
			i++;
		}
	}
	result[i] = NULL;
	return (1);
}

char	**ft_split(char *str, char delimiter)
{
	char	**result;
	int		word;

	word = word_count(str, delimiter);
	result = malloc(sizeof(char *) * (word + 1));
	if (!result)
		return (NULL);
	if (!export_word(str, result, delimiter))
	{
		free_result(result, word);
		return (NULL);
	}
	return (result);
}
