/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:59:46 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/13 16:01:28 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_no_quote(char c, int *q, size_t *i, const char *s)
{
	if (c == '\'' || c == '"')
		*q = c;
	else if (c == '\\' && s[*i + 1])
		(*i)++;
	return (*q);
}

int	handle_single_quote(char c, int *q)
{
	if (c == '\'')
		*q = 0;
	return (*q);
}

int	handle_double_quote(char c, int *q, size_t i, const char *s)
{
	if (c == '"' && (i == 0 || s[i - 1] != '\\'))
		*q = 0;
	return (*q);
}
