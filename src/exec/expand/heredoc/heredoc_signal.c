/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 22:19:39 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/29 14:14:36 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	hdoc_set_signals(void)
{
	struct sigaction	sa;

	rl_event_hook = NULL;
	ft_memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = signal_handler;
	x_sigaction(SIGINT, &sa);
	sa.sa_handler = SIG_IGN;
	x_sigaction(SIGQUIT, &sa);
}

int	hdoc_abort_sigint(int p0, int p1, char *line)
{
	if (line)
		free(line);
	write(STDOUT_FILENO, "\n", 1);
	signal_handler(0);
	set_exit_status(130);
	if (p0 >= 0)
		close(p0);
	if (p1 >= 0)
		close(p1);
	install_signal_handlers();
	return (-2);
}

int	hdoc_should_abort(int p0, int p1)
{
	if (g_signal == SIGINT)
	{
		hdoc_abort_sigint(p0, p1, NULL);
		return (1);
	}
	return (0);
}

int	hdoc_prepare(int pfd[2])
{
	if (hdoc_open_pipe(pfd) < 0)
		return (-1);
	hdoc_set_signals();
	signal_handler(0);
	return (0);
}

int	hdoc_finish_success(int pfd[2], int *out_fd)
{
	close(pfd[1]);
	*out_fd = pfd[0];
	install_signal_handlers();
	return (0);
}
