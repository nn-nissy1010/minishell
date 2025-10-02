/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_child_main.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 20:04:49 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/02 10:07:03 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_main_after_fork(t_cmd *c)
{
	int	saved_in;
	int	saved_out;
	int	st;

	saved_in = -1;
	saved_out = -1;
	set_child_signals_default();
	if (apply_redirs(c->redirs, c->n_redirs, &saved_in, &saved_out) < 0)
		exit(1);
	if (saved_in >= 0)
		close(saved_in);
	if (saved_out >= 0)
		close(saved_out);
	if (is_builtin_any(c))
	{
		st = run_builtin_child(c);
		exit(st);
	}
	run_external_in_child(c);
	exit(127);
}

pid_t	spawn_child(t_cmd *c)
{
	pid_t pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		child_main_after_fork(c);
		exit(1);
	}
	return (pid);
}