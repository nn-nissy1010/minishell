/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 21:56:42 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/07/30 12:11:32 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

char	*after_newline(char *line)
{
	char	*newline;

	if (!line)
		return (NULL);
	newline = ft_strchr(line, '\n');
	if (!newline || !*(newline + 1))
		return (NULL);
	return (ft_strdup(newline + 1));
}

char	*before_newline(char *line)
{
	size_t	i;

	i = 0;
	if (!line || *line == '\0')
		return (NULL);
	while (line[i] && line[i] != '\n')
		i++;
	if (line[i] == '\n')
		i++;
	return (ft_substr(line, 0, i));
}

static char	*read_and_append(int fd, char *save, char *buf)
{
	ssize_t	byte_read;
	char	*tmp;

	while (!save || !ft_strchr(save, '\n'))
	{
		byte_read = read(fd, buf, BUFFER_SIZE);
		if (byte_read < 0)
		{
			free(save);
			return (NULL);
		}
		if (byte_read == 0)
			break ;
		buf[byte_read] = '\0';
		tmp = ft_strjoin(save, buf);
		if (!tmp)
		{
			free(save);
			return (NULL);
		}
		free(save);
		save = tmp;
	}
	return (save);
}

char	*buf_add_line(int fd, char *save)
{
	char	*buf;
	char	*result;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (free(save), NULL);
	result = read_and_append(fd, save, buf);
	free(buf);
	if (!result || *result == '\0')
	{
		free(result);
		return (NULL);
	}
	return (result);
}

char	*get_next_line(int fd)
{
	static char	*save;
	char		*line;
	char		*next;
	char		*pre_save;

	if ((fd < 0 || BUFFER_SIZE <= 0) || (BUFFER_SIZE > MAX_BUFFER_SIZE))
		return (NULL);
	pre_save = buf_add_line(fd, save);
	if (!pre_save)
	{
		save = NULL;
		return (NULL);
	}
	line = before_newline(pre_save);
	if (!line)
	{
		free(pre_save);
		save = NULL;
		return (NULL);
	}
	next = after_newline(pre_save);
	free(pre_save);
	save = next;
	return (line);
}
