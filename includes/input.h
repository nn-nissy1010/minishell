/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:11:30 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/13 20:48:43 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

typedef enum e_more
{
	MORE_NONE = 0,
	MORE_QUOTE_S,
	MORE_QUOTE_D,
	MORE_BSLASH,
	MORE_OP
}			t_more;

int			repl(void);

void		install_signal_handlers(void);
void		install_signal_handlers_more_input(void);

const char	*get_ps2_prompt(t_more need);
int			handle_unexpected_eof(t_more need);
char		*append_line(char *acc, const char *next);

char		*read_full_command_line(char *first_line);

int			handle_no_quote(char c, int *q, size_t *i, const char *s);
int			handle_single_quote(char c, int *q);
int			handle_double_quote(char c, int *q, size_t i, const char *s);
t_more		need_more_input(const char *s);
void		x_sigaction(int signo, struct sigaction *sa);
void		signal_handler(int signo);

#endif
