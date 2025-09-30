/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_param.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:43:49 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/30 16:44:07 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_arg_part	*scan_param_part(const char **pp)
{
	const char	*p;
	const char	*start;
	t_arg_part	proto;
	t_arg_part	*part;

	p = *pp;
	start = p++;
	if (*p == '?' || *p == '$')
		p++;
	else if (ft_isdigit((unsigned char)*p))
		p++;
	else if (ft_isalpha((unsigned char)*p) || *p == '_')
    {
		p++;
		while (ft_isalnum((unsigned char)*p) || *p == '_')
			p++;
	}
    init_proto(&proto, Q_NONE);
	proto.has_param = 1;
	part = new_part_copy(start, p - start, &proto);
	if (!part)
		return (NULL);
	*pp = p;
	return (part);
}
