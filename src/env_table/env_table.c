/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 16:09:07 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/11 09:34:01 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_table	*env_table(void)
{
	static t_env_table	table = {0, 0, NULL};

	return (&table);
}
