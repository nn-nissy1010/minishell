/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:12:13 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/23 22:16:26 by tkuwahat         ###   ########.fr       */
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

int			get_exit_status(void);
void		set_exit_status(int status);
void		reset_exit_status(void);
void		inc_exit_status(int delta);

int			ft_isspace(unsigned char c);
int			is_blank_line(const char *s);
char		*ft_strjoin_3word(char *s1, const char *s2, const char *s3);
void		print_syntax_error(const char *msg);
char		*ft_strndup(const char *s, size_t n);

/* expand*/
int			expand_redirs(t_redir *redirs, size_t n);
int			expansion(t_cmd *c);
char		*parts_to_one_word(t_arg_part *parts);
int			apply_tilde_if_head_buf(t_buf *b, const char *s, t_quote_type q,
				int is_head);
char		*expand_params_part(const char *s);

/* expand_buf_utils*/
int			buf_putc(t_buf *b, char c);
int			buf_putn(t_buf *b, const void *p, size_t n);
int			buf_puts(t_buf *b, const char *s);
void		buf_clear(t_buf *b);
void		buf_free(t_buf *b);
void		buf_init(t_buf *b);
char		*buf_take(t_buf *b);
int			emit_env(t_buf *b, const char *name);
int			emit_status(t_buf *b);

// redirのテスト用
t_arg_part	*part_new(const char *text, t_quote_type q, int has_param);
t_arg_part	*chain(t_arg_part *a, t_arg_part *b);
t_token		*tok_from_parts(t_arg_part *head);
void		run_case(const char *title, t_token *word_tok);

#endif
