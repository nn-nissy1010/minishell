/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_symbol_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:46:43 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/20 16:06:34 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*scan_pipe(const char *p, t_token **t)
{
	if (*(p + 1) == '|')
	{
		*t = new_token(TOK_OR_IF, "||");
		return (p + 2);
	}
	*t = new_token(TOK_PIPE, "|");
	return (p + 1);
}

const char	*scan_and(const char *p, t_token **t)
{
	if (*(p + 1) == '&')
	{
		*t = new_token(TOK_AND_IF, "&&");
		return (p + 2);
	}
	return (p);
}

const char	*scan_redir_in(const char *p, t_token **t)
{
	if (*(p + 1) == '<')
	{
		*t = new_token(TOK_HEREDOC, "<<");
		return (p + 2);
	}
	*t = new_token(TOK_REDIR_IN, "<");
	return (p + 1);
}

const char	*scan_redir_out(const char *p, t_token **t)
{
	if (*(p + 1) == '>')
	{
		*t = new_token(TOK_REDIR_APPEND, ">>");
		return (p + 2);
	}
	*t = new_token(TOK_REDIR_OUT, ">");
	return (p + 1);
}

const char	*scan_paren(const char *p, t_token **t)
{
	if (*p == '(')
		*t = new_token(TOK_LPAR, "(");
	else
		*t = new_token(TOK_RPAR, ")");
	return (p + 1);
}
