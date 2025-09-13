/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 15:40:21 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/13 13:49:22 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

static volatile sig_atomic_t g_signal = 0;
int g_exit_status = 0;

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

static void install_signal_handlers(void)
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

int ft_isspace(unsigned char c)
{
    if (c == 32 || (c >= 9 && c <= 13))
        return 1;
    return 0;
}

int is_blank_line(const char *s) {
    while (*s) {
        if (!ft_isspace((unsigned char)*s)) {
            return 0;
        }
        s++;
    }
    return 1;
}

int repl(void){
    install_signal_handlers();
    while(1){
        char *line = readline("myshell> ");
        if (!line) {
            printf("%s", "bye!");
            break;
        }
        if (is_blank_line(line)) {
            free(line);
            continue;         
        }
        printf("入力: %s(ここで本来はコマンド処理する)\n", line);
        free(line);
    }
    return (0);
}
