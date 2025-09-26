/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_buf_reserve.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:56:19 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/25 12:56:31 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	buf_calc_newcap(size_t cur, size_t need, size_t *out_cap)
{
	size_t	ncap;

	if (need == 0)
		return (-1);
	ncap = cur;
	if (ncap == 0)
		ncap = 64;
	while (ncap < need)
	{
		if (ncap > (size_t)-1 / 2)
			return (-1);
		ncap *= 2;
	}
	*out_cap = ncap;
	return (0);
}

static int	buf_alloc_pair(size_t ncap, char **out_data,
		unsigned char **out_mask)
{
	char			*np;
	unsigned char	*nm;

	np = (char *)malloc(ncap);
	if (!np)
		return (-1);
	nm = (unsigned char *)malloc(ncap);
	if (!nm)
	{
		free(np);
		return (-1);
	}
	*out_data = np;
	*out_mask = nm;
	return (0);
}

static void	buf_copy_and_swap(t_buf *b, char *np, unsigned char *nm,
		size_t ncap)
{
	if (b->len > 0 && b->data)
		ft_memcpy(np, b->data, b->len + 1);
	else
		np[0] = '\0';
	if (b->len > 0 && b->qmask)
		ft_memcpy(nm, b->qmask, b->len);
	if (b->data)
		free(b->data);
	if (b->qmask)
		free(b->qmask);
	b->data = np;
	b->qmask = nm;
	b->cap = ncap;
}

static int	buf_qrealloc_to(t_buf *b, size_t need)
{
	size_t			ncap;
	char			*np;
	unsigned char	*nm;

	if (!b)
		return (-1);
	if (buf_calc_newcap(b->cap, need, &ncap) < 0)
		return (-1);
	if (buf_alloc_pair(ncap, &np, &nm) < 0)
		return (-1);
	buf_copy_and_swap(b, np, nm, ncap);
	return (0);
}

int	buf_qreserve(t_buf *b, size_t add)
{
	size_t need;

	if (!b)
		return (-1);
	if (add == 0)
		return (0);
	if (b->cap >= b->len + add + 1)
		return (0);
	need = b->len + add + 1;
	return (buf_qrealloc_to(b, need));
}