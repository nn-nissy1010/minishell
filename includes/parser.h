/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:13:28 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/04 23:34:54 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER
# define PARSER

typedef struct s_parser {
    t_token *cur;
}   t_parser;

int match(t_parser *p, t_tokentype type);
t_token *consume(t_parser *p);
t_node *new_node(t_ntype type);

int parse_redir_token(t_node *node, t_parser *p);

t_node *parse_expr(struct s_parser *p);
t_node *parse_and_or(struct s_parser *p);
t_node *parse_pipeline(struct s_parser *p);
t_node *parse_command(struct s_parser *p);
t_node *parse_subshell(struct s_parser *p);
t_node *parse_simple_command(struct s_parser *p);
t_node *parse(t_token *toklist);

void destroy_ast(t_node *n);

t_node **ast_ref(void);
void set_ast(t_node *new_ast);
t_node *get_ast(void);

#endif
