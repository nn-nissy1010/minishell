/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:12:13 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/18 19:41:57 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SHELL
# define MINI_SHELL

#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdlib.h>
#include <stdint.h>

#include "input.h"
#include "lexer.h"
#include "parser.h"
#include "libft.h"
#include "env_table.h"

int  get_exit_status(void);
void set_exit_status(int status);
void reset_exit_status(void);
void inc_exit_status(int delta);

int ft_isspace(unsigned char c);
int is_blank_line(const char *s);
char	*ft_strjoin_3word(char *s1, const char *s2, const char *s3);
void	print_syntax_error(const char *msg);
char *ft_strndup(const char *s, size_t n);

#endif
