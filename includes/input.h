/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:11:30 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/18 19:41:52 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT
# define INPUT

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

#endif
