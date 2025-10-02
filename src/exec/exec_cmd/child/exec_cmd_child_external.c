/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_child_external.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 22:56:31 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/02 10:10:42 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_slash(const char *s)
{
	if (!s)
		return (0);
	if (ft_strchr(s, '/'))
		return (1);
	return (0);
}

void	run_external_in_child(t_cmd *c)
{
	if (!c || !c->argv || !c->argv[0])
		exit(0);
	if (has_slash(c->argv[0]))
		exec_direct(c->argv);
	else
		search_and_exec(c->argv);
	exit(127);
}
