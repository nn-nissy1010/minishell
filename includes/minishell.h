/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:12:13 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/22 18:41:27 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SHELL
# define MINI_SHELL

# include "env_table.h"
# include "exec.h"
# include "input.h"
# include "lexer.h"
# include "libft.h"
# include "parser.h"
# include <ctype.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

int		get_exit_status(void);
void	set_exit_status(int status);
void	reset_exit_status(void);
void	inc_exit_status(int delta);

int		ft_isspace(unsigned char c);
int		is_blank_line(const char *s);
char	*ft_strjoin_3word(char *s1, const char *s2, const char *s3);
void	print_syntax_error(const char *msg);
char	*ft_strndup(const char *s, size_t n);

#endif
