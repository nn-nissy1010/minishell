/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:52:18 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/13 16:09:00 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile sig_atomic_t g_signal = 0;

static void signal_handler(int signo) {
    g_signal = signo;
}

static int on_readline_event(void) {
    if (g_signal == SIGINT) {
        g_signal = 0;
        g_exit_status = 130;

        write(STDOUT_FILENO, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
    return 0;
}

static void x_sigaction(int signo, struct sigaction *sa)
{
    if (sigaction(signo, sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}

void install_signal_handlers(void)
{
    struct sigaction sa;

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
