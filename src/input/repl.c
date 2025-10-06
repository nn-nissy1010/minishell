/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 15:40:21 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/06 15:38:45 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*read_command(void)
{
	char	*line;
	char	*full;

	line = readline("myshell> ");
	if (!line)
		return (NULL);
	if (is_blank_line(line))
	{
		free(line);
		return (NULL);
	}
	full = read_full_command_line(line);
	if (!full)
		return (NULL);
	return (full);
}

static int process_command(char *full)
{
	set_tokens(lexer(full));
	set_ast(parse(get_tokens()));
	if (!get_ast())
	{
		free_tokens(get_tokens());
		free(full);
		return (0);
	}
	return (1);
}

static int	execute_command(t_exec_ctx *ctx)
{
	int	rc;

	rc = ast_exec(get_ast(), ctx);
	printf("rc=%d\n", rc);
	return (rc);
}

static void	cleanup_command(char *full)
{
	destroy_ast(get_ast());
	free_tokens(get_tokens());
	free(full);
}

int	repl(void)
{
	char		*full;
	t_exec_ctx	*ctx;

	install_signal_handlers();
	while (1)
	{
		full = read_command();
		if (!full)
		{
			if (!isatty(0))
				break ;
			printf("bye! exitcode : %d\n", get_exit_status());
			break ;
		}
		if (!process_command(full))
			continue ;
		ctx = NULL;
		execute_command(ctx);
		cleanup_command(full);
	}
	return (0);
}
