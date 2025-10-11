/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 06:30:02 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/05/07 11:45:24 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;

	if (!*lst || !del)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		del((*lst)->content);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

// static void	del_content(void *content)
// {
// 	free(content);
// }
// #include <string.h>

// int	main(void)
// {
// 	t_list	*node1;
// 	char	*str;

// 	str = malloc(8);
// 	if (!str)
// 		return (1);
// 	strcpy(str, "abcdefg");
// 	node1 = ft_lstnew(str);
// 	ft_lstclear(node1, del_content);
// }
