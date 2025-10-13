/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_redir_tty.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 19:57:16 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/13 18:41:51 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bind_child_readline_to_tty(void)
{
	int	tty;

	tty = open("/dev/tty", O_RDWR);
	if (tty >= 0)
	{
		(void)dup2(tty, STDIN_FILENO);
		(void)dup2(tty, STDERR_FILENO);
		close(tty);
	}
	rl_instream = stdin;
	rl_outstream = stderr;
}

static void	drain_stdin_bytes(void)
{
	char	buf[256];
	ssize_t	n;

	n = 1;
	while (n > 0)
	{
		n = read(STDIN_FILENO, buf, sizeof buf);
		if (n < 0)
		{
			if (errno == EINTR)
				n = 1;
			else
				break ;
		}
	}
}

void	ms_drain_tty_input(void)
{
	struct termios	org;
	struct termios	tmp;

	if (!isatty(STDIN_FILENO))
		return ;
	if (tcgetattr(STDIN_FILENO, &org) < 0)
		return ;
	tmp = org;
	tmp.c_lflag &= ~(ICANON | ECHO);
	tmp.c_cc[VMIN] = 0;
	tmp.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &tmp) < 0)
		return ;
	drain_stdin_bytes();
	(void)tcsetattr(STDIN_FILENO, TCSANOW, &org);
}

void	tty_force_canonical_echo_isig(void)
{
	struct termios	t;

	if (!isatty(STDIN_FILENO))
		return ;
	if (tcgetattr(STDIN_FILENO, &t) < 0)
		return ;
	t.c_iflag |= ICRNL;
	t.c_lflag |= (ICANON | ECHO | ISIG);
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 0;
	(void)tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void	rebind_tty_if_needed(void)
{
	int	fd;

	if (!isatty(STDIN_FILENO))
	{
		fd = open("/dev/tty", O_RDONLY);
		if (fd >= 0 && fd != STDIN_FILENO)
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
	}
	if (!isatty(STDOUT_FILENO))
	{
		fd = open("/dev/tty", O_WRONLY);
		if (fd >= 0 && fd != STDOUT_FILENO)
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
	}
}
