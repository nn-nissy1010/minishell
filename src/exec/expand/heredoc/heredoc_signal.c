/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 22:19:39 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/06 17:21:53 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	hdoc_should_abort(int p0, int p1)
{
	(void)p0;
	(void)p1;
	return (g_signal == SIGINT);
}

int	hdoc_prepare(int pfd[2])
{
	if (hdoc_open_pipe(pfd) < 0)
		return (-1);
	hdoc_set_signals();
	hdoc_reset_signal_state();
	return (0);
}

int	hdoc_finish_success(int pfd[2], int *out_fd)
{
	close(pfd[1]);
	*out_fd = pfd[0];
	hdoc_restore_after();
	return (0);
}
