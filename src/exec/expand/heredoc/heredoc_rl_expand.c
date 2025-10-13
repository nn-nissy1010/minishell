/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_rl_expand.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 22:17:53 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/13 11:10:51 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	hdoc_open_pipe(int pfd[2])
{
	if (pipe(pfd) < 0)
		return (-1);
	return (0);
}

int	hdoc_write_line(int fd, const char *s)
{
	ssize_t	w;

	if (!s)
		return (-1);
	w = write(fd, s, ft_strlen(s));
	if (w < 0)
		return (-1);
	w = write(fd, "\n", 1);
	if (w < 0)
		return (-1);
	return (0);
}

char	*hdoc_maybe_expand(const char *line, int quoted)
{
	char	*out;

	if (line == NULL)
		return (ft_strdup(""));
	if (quoted)
		return (ft_strdup(line));
	out = expand_params_part(line);
	return (out);
}
