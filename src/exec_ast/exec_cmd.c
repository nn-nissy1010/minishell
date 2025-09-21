
// #include "../../includes/minishell.h"

#include "./../../includes/exec.h"
#include <stdio.h>
#include <stdlib.h>

int	cmd_exec(t_ast *self, t_exec_ctx *ctx)
{
	t_cmd	*c;
	int		rc;

	c = &self->as.cmd;
	if (build_argv_from_tokens(c->argv_tokens, &c->argv) != 0)
	{
		ctx->last_status = 1;
		return (1);
	}
	c->argc = 0;
	if (c->argv)
		while (c->argv[c->argc])
			c->argc++;
	for (size_t i = 0; i < c->n_redirs; i++)
	{
		if (expand_redir_target(c->redirs[i].word, &c->redirs[i].path) != 0)
		{
			fprintf(stderr, "minishell: ambiguous redirect\n");
			ctx->last_status = 1;
			return (1);
		}
	}
	rc = run_command(c->argv, c->redirs, c->n_redirs, ctx);
	ctx->last_status = rc;
	return (rc);
}
void	cmd_destroy(t_ast *self)
{
	t_cmd	*c;

	c = &self->as.cmd;
	if (c->argv)
	{
		for (size_t i = 0; c->argv[i]; i++)
			free(c->argv[i]);
		free(c->argv);
	}
	for (size_t i = 0; i < c->n_redirs; i++)
	{
		free(c->redirs[i].path); // 展開後
									/* c->redirs[i].word はトークン所有側で解放する方針でもOK */
	}
	free(c->redirs);
	/* ここで argv_tokens を解放する設計なら：
		free_tokens(c->argv_tokens); */
	free(self);
}
