/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 15:40:21 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/11 09:35:04 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*read_command(void)
{
	char	*line;
	char	*full;
	char	*empty;

	sanitize_before_prompt();
	line = readline("myshell> ");
	if (!line)
		return (NULL);
	if (is_blank_line(line))
	{
		empty = ft_strdup("");
		free(line);
		return (empty);
	}
	full = read_full_command_line(line);
	if (!full)
	{
		free(line);
		return (NULL);
	}
	if (full != line)
		free(line);
	return (full);
}

static int	process_command(char *full)
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
			if (isatty(0))
				printf("bye! exitcode : %d\n", get_exit_status());
			break ;
		}
		if (*full == '\0')
		{
			free(full);
			continue ;
		}
		if (!process_command(full))
			continue ;
		ctx = NULL;
		execute_command(ctx);
		cleanup_command(full);
	}
	return (0);
}
