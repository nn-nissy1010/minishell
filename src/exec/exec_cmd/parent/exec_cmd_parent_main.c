/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_parent_main.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 22:31:24 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/12 12:30:42 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int    run_builtin_parent(t_cmd *c)
{
    char    **av;

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
    {
        if(c->argc == 1 || c->argc == 2)
            return (bi_exit(av));
        else if(exit_validate(av[1]) == 0)
            return (bi_exit(av));
        else
        {
            write(STDOUT_FILENO, "exit: too many arguments\n", 25);
            return (1);
        }
    }
    return (-1);
}

int	wait_and_status(pid_t pid)
{
	int	st;
	int	sig;

	if (waitpid_retry(pid, &st) < 0)
		return (-1);
	if (WIFEXITED(st))
		return (WEXITSTATUS(st));
	if (WIFSIGNALED(st))
	{
		sig = WTERMSIG(st);
		if (sig == SIGINT)
			write(1, "\n", 1);
		else if (sig == SIGQUIT)
			write(1, "Quit: 3\n", 8);
		return (128 + sig);
	}
	return (1);
}

int	wait_child_status(pid_t pid, int *out_st)
{
	int	st;

	if (!out_st)
		return (-1);
	while (1)
	{
		if (waitpid(pid, &st, 0) >= 0)
			break ;
		if (errno == EINTR)
			continue ;
		perror("waitpid");
		return (-1);
	}
	*out_st = st;
	return (0);
}

int	parent_finalize_simple(pid_t pid)
{
	int	st;
	int	sig;

	if (wait_child_status(pid, &st) < 0)
		return (set_exit_status(1), -1);
	if (WIFEXITED(st))
		return (set_exit_status(WEXITSTATUS(st)), 0);
	if (WIFSIGNALED(st))
	{
		sig = WTERMSIG(st);
		if (sig == SIGINT)
		{
			write(STDERR_FILENO, "\n", 1);
			signal_handler(0);
		}
		else if (sig == SIGQUIT)
		{
			write(STDERR_FILENO, "Quit: 3\n", 8);
			signal_handler(0);
		}
		return (set_exit_status(128 + sig), 0);
	}
	return (set_exit_status(1), 0);
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
	rebind_tty_if_needed();
	rl_instream = stdin;
	rl_outstream = stdout;
	destroy_cmd_min_cmd(c);
	set_exit_status(status & 0xFF);
	return (0);
}
