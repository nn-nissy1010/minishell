/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:14:05 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/11 09:38:10 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	*exit_status_ref(void)
{
	static int	exit_status = 0;

	return (&exit_status);
}

int	get_exit_status(void)
{
	return (*exit_status_ref());
}

void	set_exit_status(int status)
{
	*exit_status_ref() = status;
}

void	reset_exit_status(void)
{
	*exit_status_ref() = 0;
}

void	inc_exit_status(int delta)
{
	*exit_status_ref() += delta;
}
