
#include "./../../includes/exec.h"
#include <stdio.h>  // printf
#include <stdlib.h> // free

int	and_exec(t_ast *self, t_exec_ctx *ctx)
{
	int	rc;

	printf("[exec] AND_IF   : (left && right)\n");
	rc = ast_exec(self->as.bin.left, ctx);
	if (rc == 0)
	{
		rc = ast_exec(self->as.bin.right, ctx);
	}
	else
	{
		printf("       short-circuit: left!=0 -> skip right\n");
	}
	ctx->last_status = rc;
	printf("[ret ] AND_IF   : rc=%d\n", rc);
	return (rc);
}

void	and_destroy(t_ast *self)
{
	printf("[free] AND_IF\n");
	ast_destroy(self->as.bin.left);
	ast_destroy(self->as.bin.right);
	free(self);
}
