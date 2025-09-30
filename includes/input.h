/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:11:30 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/29 16:12:06 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT
# define INPUT

typedef enum e_more
{
	MORE_NONE = 0,
	MORE_QUOTE_S,
	MORE_QUOTE_D,
	MORE_BSLASH,
	MORE_OP
}		t_more;

int		repl(void);

void	install_signal_handlers(void);

char	*read_full_command_line(char *first_line);

int		handle_no_quote(char c, int *q, size_t *i, const char *s);
int		handle_single_quote(char c, int *q);
int		handle_double_quote(char c, int *q, size_t i, const char *s);
t_more	need_more_input(const char *s);
void	x_sigaction(int signo, struct sigaction *sa);
void	signal_handler(int signo);

#endif
