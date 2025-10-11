/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operate_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 21:45:14 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/11 09:36:45 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	**tokens_ref(void)
{
	static t_token	*tokens = NULL;

	return (&tokens);
}

void	set_tokens(t_token *new_tokens)
{
	*tokens_ref() = new_tokens;
}

t_token	*get_tokens(void)
{
	return (*tokens_ref());
}
