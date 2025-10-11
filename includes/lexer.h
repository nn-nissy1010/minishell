/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:13:01 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/11 10:04:02 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

typedef enum e_tokentype
{
	TOK_ARG,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_REDIR_APPEND,
	TOK_HEREDOC,
	TOK_AND_IF,
	TOK_OR_IF,
	TOK_LPAR,
	TOK_RPAR,
	TOK_EOF
}						t_tokentype;

typedef enum e_quote_type
{
	Q_NONE = 0,
	Q_SINGLE = 1,
	Q_DOUBLE = 2
}						t_quote_type;

typedef struct s_arg_part
{
	char				*text;
	t_quote_type		quote;
	int					has_param;
	int					has_unq_glob;
	struct s_arg_part	*next;
}						t_arg_part;

typedef struct s_arg
{
	char				*raw;
	t_arg_part			*parts;
	char				**items;
	size_t				n_items;
	int					expanded;
}						t_arg;

typedef struct s_heredoc
{
	int					heredoc_quoted;
}						t_heredoc;

typedef struct s_token
{
	t_tokentype			type;
	union
	{
		t_arg			arg;
		t_heredoc		heredoc;
	} u_u;
	struct s_token		*next;
}						t_token;

typedef struct s_buf
{
	char				*data;
	size_t				cap;
	size_t				len;
	unsigned char		*qmask;
}						t_buf;

typedef struct s_argbuf
{
	char				**v;
	size_t				n;
	size_t				cap;
}						t_argbuf;

t_token					*new_token_n(t_tokentype type, const char *s, size_t n);
t_token					*new_token(t_tokentype type, const char *s);
void					tok_push(t_token **head, t_token **tail, t_token *node);
void					free_tokens(t_token *t);

void					init_proto(t_arg_part *proto, t_quote_type quote);
void					init_scan(t_buf *b, t_arg_part *proto,
							t_quote_type quote);

t_arg_part				*new_part_copy(const char *s, size_t len,
							const t_arg_part *proto);
t_arg_part				*scan_quoted_part(const char **pp);
t_arg_part				*scan_unquoted_part(const char **pp);
t_arg_part				*scan_param_part(const char **pp);

const char				*scan_pipe(const char *p, t_token **t);
const char				*scan_and(const char *p, t_token **t);
const char				*scan_redir_in(const char *p, t_token **t);
const char				*scan_redir_out(const char *p, t_token **t);
const char				*scan_paren(const char *p, t_token **t);

int						finalize_arg(t_arg *arg);
void					free_parts(t_arg_part *parts);
void					append_part(t_arg_part **head, t_arg_part *node);

char					*append_char(char *buf, size_t *cap, size_t *len,
							char ch);

t_token					*lexer(const char *input);

t_token					**tokens_ref(void);
void					set_tokens(t_token *new_tokens);
t_token					*get_tokens(void);

#endif
