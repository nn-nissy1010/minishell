/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_bi_unset.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:02:28 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/01 21:52:41 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_unset(char **av)
{
	int	i;
	int	rc;

	if (!av)
		return (0);
	i = 1;
	rc = 0;
	while (av[i])
	{
		if (!is_valid_ident(av[i]))
		{
			err3("minishell: unset: `", av[i], "': not a valid identifier\n");
			rc = 1;
		}
		else
		{
			(void)unset_env_table(av[i]);
		}
		i++;
	}
	return (rc);
}
