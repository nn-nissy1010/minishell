/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:14:05 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/18 11:14:38 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status = 0;

int get_exit_status(void)
{
    return (g_exit_status);
}

void set_exit_status(int status)
{
    g_exit_status = status;
}

void reset_exit_status(void)
{
    g_exit_status = 0;
}

void inc_exit_status(int delta)
{
    g_exit_status += delta;
}
