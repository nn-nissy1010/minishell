/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_signal_manage.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:30:08 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/13 17:03:47 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	safe_close(int fd)
{
	if (fd >= 0)
		(void)close(fd);
}

int	waitpid_retry(pid_t pid, int *st)
{
	int	rc;

	while (1)
	{
		rc = waitpid(pid, st, 0);
		if (rc < 0 && errno == EINTR)
			continue ;
		return (rc);
	}
}

int	status_to_exitcode(int st)
{
	if (WIFEXITED(st))
		return (WEXITSTATUS(st));
	if (WIFSIGNALED(st))
		return (128 + WTERMSIG(st));
	return (1);
}

void	reset_child_signals(void)
{
	struct sigaction	dfl;

	ft_memset(&dfl, 0, sizeof(dfl));
	dfl.sa_handler = SIG_DFL;
	sigemptyset(&dfl.sa_mask);
	dfl.sa_flags = 0;
	x_sigaction(SIGINT, &dfl);
	dfl.sa_handler = SIG_IGN;
	x_sigaction(SIGQUIT, &dfl);
}

int	parent_mask_sigint(struct sigaction *old)
{
	struct sigaction	ign;

	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	ign.sa_handler = SIG_IGN;
	return (sigaction(SIGINT, &ign, old));
}
