/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 22:15:50 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/02 14:16:20 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_builtin_child(t_cmd *c)
{
	char	**av;

	if (!c || !c->argv || !c->argv[0])
		return (0);
	av = c->argv;
	if (ft_strcmp(av[0], "echo") == 0)
		return (bi_echo(av));
	if (ft_strcmp(av[0], "pwd") == 0)
		return (bi_pwd(av));
	if (ft_strcmp(av[0], "env") == 0)
		return (bi_env(av));
	if (ft_strcmp(av[0], "cd") == 0)
		return (bi_cd(av));
	if (ft_strcmp(av[0], "export") == 0)
		return (bi_export(av));
	if (ft_strcmp(av[0], "unset") == 0)
		return (bi_unset(av));
	if (ft_strcmp(av[0], "exit") == 0)
		return (bi_exit(av));
	return (127);
}

int	is_builtin_any(const t_cmd *c)
{
	char	*b;

	if (!c || !c->argv || !c->argv[0])
		return (0);
	b = c->argv[0];
	if (ft_strcmp(b, "cd") == 0
		|| ft_strcmp(b, "export") == 0
		|| ft_strcmp(b, "unset") == 0
		|| ft_strcmp(b, "exit") == 0
		|| ft_strcmp(b, "echo") == 0
		|| ft_strcmp(b, "pwd") == 0
		|| ft_strcmp(b, "env") == 0)
		return (1);
	return (0);
}

void	set_child_signals_default(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_DFL;
	x_sigaction(SIGINT, &sa);
	x_sigaction(SIGQUIT, &sa);
}
