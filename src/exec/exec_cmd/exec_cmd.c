/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 22:09:18 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/23 22:09:26 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_func	*v_cmd(void)
{
	static t_func	f = {exec_cmd, destroy_cmd_min};

	return (&f);
}
int	exec_cmd(t_node *node, t_exec_ctx *ctx)
{
	t_cmd	*c;
	int		rc;

	c = &node->as.cmd;
	rc = expansion(c);
	return (0);
}

void	destroy_cmd_min(t_node *node)
{
	if (!node)
		return ;
	free_token_list(node->as.cmd.argv_tokens);
}
