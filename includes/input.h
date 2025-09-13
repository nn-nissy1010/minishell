/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:11:30 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/13 16:07:41 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdlib.h>

extern int g_exit_status;

typedef enum e_more {
    MORE_NONE = 0,
    MORE_QUOTE_S,
    MORE_QUOTE_D,
    MORE_OP
} t_more;

int repl(void);

void install_signal_handlers(void);

char *read_full_command_line(char *first_line);

int	handle_no_quote(char c, int *q, size_t *i, const char *s);
int	handle_single_quote(char c, int *q);
int	handle_double_quote(char c, int *q, size_t i, const char *s);
t_more need_more_input(const char *s);

int ft_isspace(unsigned char c);
int is_blank_line(const char *s);
char	*ft_strjoin_3word(char *s1, const char *s2, const char *s3);
void	print_syntax_error(const char *msg);
