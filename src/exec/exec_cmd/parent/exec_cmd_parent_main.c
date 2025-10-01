/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_parent_main.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 22:31:24 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/01 22:01:48 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_builtin_parent(t_cmd *c)
{
	char	**av;

	if (!c || !c->argv || !c->argv[0])
		return (0);
	av = c->argv;
	if (ft_strcmp(av[0], "cd") == 0)
		return (bi_cd(av));
	if (ft_strcmp(av[0], "export") == 0)
		return (bi_export(av));
	if (ft_strcmp(av[0], "unset") == 0)
		return (bi_unset(av));
	if (ft_strcmp(av[0], "exit") == 0)
		return (bi_exit(av));
	return (-1);
}

static int	parent_finalize(pid_t pid, int saved_in, int saved_out)
{
	int	status;

	restore_stdio(saved_in, saved_out);
	status = wait_and_status(pid);
	set_exit_status(status);
	return (0);
}

int	run_parent_builtin_flow(t_cmd *c)
{
	int	saved_in;
	int	saved_out;
	int	status;

	saved_in = -1;
	saved_out = -1;
	if (apply_redirs(c->redirs, c->n_redirs, &saved_in, &saved_out) < 0)
		return (set_exit_status(1), -1);
	status = run_builtin_parent(c);
	restore_stdio(saved_in, saved_out);
	set_exit_status(status & 0xFF);
	return (0);
}
