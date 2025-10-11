/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parts_manage.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:58:13 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/11 09:35:18 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_arg_part	*new_part_copy(const char *s, size_t len, const t_arg_part *proto)
{
	t_arg_part	*p;

	p = malloc(sizeof(*p));
	if (!p)
		return (NULL);
	p->text = ft_strndup(s, len);
	if (!p->text)
	{
		free(p);
		return (NULL);
	}
	p->quote = proto->quote;
	p->has_param = proto->has_param;
	p->has_unq_glob = proto->has_unq_glob;
	p->next = NULL;
	return (p);
}

void	append_part(t_arg_part **head, t_arg_part *node)
{
	t_arg_part	*cur;

	if (!node)
		return ;
	if (!*head)
	{
		*head = node;
	}
	else
	{
		cur = *head;
		while (cur->next)
			cur = cur->next;
		cur->next = node;
	}
}

void	free_parts(t_arg_part *parts)
{
	t_arg_part	*next;

	while (parts)
	{
		next = parts->next;
		free(parts->text);
		free(parts);
		parts = next;
	}
}
