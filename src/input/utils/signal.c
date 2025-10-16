/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:52:18 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/16 10:42:58 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

void	signal_handler(int signo)
{
	g_signal = signo;
}

void	x_sigaction(int signo, struct sigaction *sa)
{
	if (sigaction(signo, sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}
}
