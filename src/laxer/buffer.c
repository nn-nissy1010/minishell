/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:40:13 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/20 16:13:47 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *append_char(char *buf, size_t *cap, size_t *len, char ch)
{
    size_t newcap;
    char   *tmp;

    if (*len + 1 >= *cap)
    {
        if (*cap == 0)
            newcap = 32;
        else
            newcap = *cap * 2;
        tmp = malloc(newcap);
        if (!tmp)
            return (perror("malloc"),free(buf), NULL);
        if (buf)
        {
            ft_memcpy(tmp, buf, *len);
            free(buf);
        }
        buf = tmp;
        *cap = newcap;
    }
    buf[(*len)++] = ch;
    return buf;
}
