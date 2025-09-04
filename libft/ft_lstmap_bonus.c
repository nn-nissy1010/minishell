/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 09:43:26 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/07/30 12:07:02 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// static void	*to_upper(void *content)
// {
// 	char	*str;
// 	char	*copy;
// 	int		i;

// 	str = (char *)content;
// 	copy = ft_strdup(str);
// 	i = 0;
// 	if (!copy)
// 		return (NULL);
// 	i = 0;
// 	while (copy[i])
// 	{
// 		copy[i] = copy[i] - 32;
// 		i++;
// 	}
// 	return ((void *)copy);
// }

// static void	del_content(void *content)
// {
// 	free(content);
// }

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_node;
	t_list	*new_list;
	void	*new_content;

	if (!lst || !f)
		return (NULL);
	new_list = NULL;
	while (lst)
	{
		new_content = f(lst->content);
		if (!new_content)
		{
			return (ft_lstclear(&new_list, del), NULL);
		}
		new_node = ft_lstnew(new_content);
		if (!new_node)
		{
			del(new_content);
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		ft_lstadd_back(&new_list, new_node);
		lst = lst->next;
	}
	return (new_list);
}
