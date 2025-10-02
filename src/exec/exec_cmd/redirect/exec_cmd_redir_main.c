/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_redir_main.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 19:57:16 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/01 20:25:20 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	apply_redirs(t_redir *r, size_t n, int *saved_in, int *saved_out)
{
	size_t	i;

	*saved_in = -1;
	*saved_out = -1;
	if (pre_backup(r, n, saved_in, saved_out) < 0)
		return (-1);
	i = 0;
	while (i < n)
	{
		if (apply_one_redir(&r[i]) < 0)
		{
			rollback_and_invalidate(saved_in, saved_out);
			return (-1);
		}
		i++;
	}
	return (0);
}

int	redirect_only_flow(t_cmd *c)
{
	int	saved_in;
	int	saved_out;

	saved_in = -1;
	saved_out = -1;
	if (apply_redirs(c->redirs, c->n_redirs, &saved_in, &saved_out) < 0)
		return (set_exit_status(1), -1);
	restore_stdio(saved_in, saved_out);
	return (set_exit_status(0), 0);
}
