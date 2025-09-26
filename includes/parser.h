/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:13:28 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/24 20:20:52 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER
# define PARSER

typedef struct s_parser {
    t_token *cur;
}   t_parser;

t_node *parse_expr(struct s_parser *p);
t_node *parse_and_or(struct s_parser *p);
t_node *parse_pipeline(struct s_parser *p);
t_node *parse_command(struct s_parser *p);
t_node *parse_subshell(struct s_parser *p);
t_node *parse_simple_command(struct s_parser *p);
t_node *parse(t_token *toklist);

void print_ast(const t_node *n, int depth);
void destroy(t_node *n);

#endif
