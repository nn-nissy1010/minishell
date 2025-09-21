/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_quote_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:45:39 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/20 16:03:51 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_proto(t_arg_part *proto, t_quote_type quote)
{
	proto->quote = quote;
	proto->has_param = 0;
	proto->has_unq_glob = 0;
}

void	init_scan(t_buf *b, t_arg_part *proto, t_quote_type quote)
{
	b->data = NULL;
	b->cap = 0;
	b->len = 0;
	init_proto(proto, quote);
}
