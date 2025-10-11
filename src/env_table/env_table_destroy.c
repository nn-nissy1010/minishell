/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_table_destroy.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 15:03:25 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/11 09:33:48 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	destroy_env_table_impl(t_env_table *t)
{
	size_t	i;

	if (!t || !t->pair)
		return ;
	i = 0;
	while (i < t->cap)
	{
		free_entry(&t->pair[i]);
		i++;
	}
	free(t->pair);
	t->pair = NULL;
	t->cap = 0;
	t->size = 0;
	*t = (t_env_table){0};
}

void	destroy_env_table(void)
{
	t_env_table	*t;

	t = env_table();
	destroy_env_table_impl(t);
}
