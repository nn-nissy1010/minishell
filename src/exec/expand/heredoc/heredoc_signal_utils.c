/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signal_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 22:19:39 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/06 17:53:44 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	hdoc_reset_signal_state(void)
{
	g_signal = 0;
}

void	hdoc_restore_after(void)
{
	install_signal_handlers();
}

void	hdoc_set_signals(void)
{
	struct sigaction	sa;

	rl_event_hook = NULL;
	rl_catch_signals = 0;
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
	set_exit_status(130);
	if (p0 >= 0)
		close(p0);
	if (p1 >= 0)
		close(p1);
	hdoc_reset_signal_state();
	return (-2);
}
