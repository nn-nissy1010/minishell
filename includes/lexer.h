/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:13:01 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/18 20:23:51 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LAXER
# define LAXER

typedef enum e_tokentype {
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
} t_tokentype;

typedef struct s_arg {
    char   *raw;
    char  **items;
    size_t  n_items;
    int     has_param;
    int     saw_quote;
    int     has_unq_glob;
    int     expanded;
} t_arg;

typedef struct s_heredoc {
    int heredoc_quoted;
} t_heredoc;


typedef struct s_token {
    t_tokentype type;
    union {
        t_arg     arg;
        t_heredoc heredoc;
    } u;
    struct s_token *next;
} t_token;

t_token *new_token_n(t_tokentype type, const char *s, size_t n);
t_token *new_token(t_tokentype type, const char *s);
void tok_push(t_token **head, t_token **tail, t_token *node);
void free_tokens(t_token *t);

t_token *lexer(const char *input);






#endif
