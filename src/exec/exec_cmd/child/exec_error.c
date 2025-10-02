/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 22:08:30 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/02 19:30:20 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	err3(const char *prefix, const char *subject, const char *message)
{
	char	buf[512];
	size_t	n;

	buf[0] = '\0';
	n = 0;
	buf_cat(buf, &n, sizeof(buf), prefix);
	buf_cat(buf, &n, sizeof(buf), subject);
	buf_cat(buf, &n, sizeof(buf), message);
	(void)write(STDERR_FILENO, buf, n);
}

void	exit_enoexec(const char *cmd)
{
	err3("minishell: ", cmd, ": Exec format error\n");
	exit(126);
}
