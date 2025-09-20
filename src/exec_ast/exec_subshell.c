

#include "./../../includes/exec.h"
#include <stdio.h>
#include <stdlib.h>

int	subshell_exec(t_ast *self, t_exec_ctx *ctx)
{
	int	rc;

	printf("[exec] SUBSHELL : ( ... )  // デモではfork無し\n");
	rc = ast_exec(self->as.subshell.body, ctx);
	ctx->last_status = rc;
	printf("[ret ] SUBSHELL : rc=%d\n", rc);
	return (rc);
}

void	subshell_destroy(t_ast *self)
{
	printf("[free] SUBSHELL\n");
	ast_destroy(self->as.subshell.body);
	free(self);
}