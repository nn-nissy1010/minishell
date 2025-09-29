/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_sort.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:26:50 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/29 12:29:50 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	swap_strptr(char **a, char **b)
{
	char	*t;

	t = *a;
	*a = *b;
	*b = t;
}

static int	cmp_strptr_raw(const char *sa, const char *sb)
{
	if (sa == NULL)
		sa = "";
	if (sb == NULL)
		sb = "";
	return (ft_strcmp(sa, sb));
}

static void	heapify_strptr(char **v, size_t n, size_t i)
{
	size_t	largest;
	size_t	l;
	size_t	r;

	while (1)
	{
		largest = i;
		l = i * 2 + 1;
		r = i * 2 + 2;
		if (l < n && cmp_strptr_raw(v[l], v[largest]) > 0)
			largest = l;
		if (r < n && cmp_strptr_raw(v[r], v[largest]) > 0)
			largest = r;
		if (largest == i)
			break ;
		swap_strptr(&v[i], &v[largest]);
		i = largest;
	}
}

void	sort_strptrs(char **v, size_t n)
{
	size_t	i;

	if (!v || n < 2)
		return ;
	i = n / 2;
	while (i > 0)
	{
		i--;
		heapify_strptr(v, n, i);
	}
	i = n;
	while (i > 1)
	{
		i--;
		swap_strptr(&v[0], &v[i]);
		heapify_strptr(v, i, 0);
	}
}
