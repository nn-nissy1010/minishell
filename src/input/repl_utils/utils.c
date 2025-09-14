/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:53:19 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/13 16:03:14 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_isspace(unsigned char c)
{
    if (c == 32 || (c >= 9 && c <= 13))
        return 1;
    return 0;
}

int is_blank_line(const char *s)
{
    while (*s) {
        if (!ft_isspace((unsigned char)*s)) {
            return 0;
        }
        s++;
    }
    return 1;
}

char	*ft_strjoin_3word(char *s1, const char *s2, const char *s3)
{
	char *tmp;
	char *res;

	if (!s1 || !s2 || !s3)
		return (NULL);
	tmp = ft_strjoin(s1, s2);
	free(s1);
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}

void	print_syntax_error(const char *msg)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
}
