/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   install_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 10:37:41 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/16 10:43:09 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	signal_handler_more_input(int sig)
{
	g_signal = sig;
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		rl_done = 1;
		set_exit_status(130);
	}
}

static int	on_readline_event(void)
{
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		set_exit_status(130);
		write(STDERR_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	return (0);
}

void	install_signal_handlers(void)
{
	struct sigaction	sa;

	rl_catch_signals = 0;
	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	x_sigaction(SIGINT, &sa);
	sa.sa_handler = SIG_IGN;
	x_sigaction(SIGQUIT, &sa);
	rl_event_hook = on_readline_event;
}

void	install_signal_handlers_more_input(void)
{
	struct sigaction	sa;

	rl_catch_signals = 0;
	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = signal_handler_more_input;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	x_sigaction(SIGINT, &sa);
}
