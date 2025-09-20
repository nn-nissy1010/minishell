

#include "./../../includes/exec.h"
#include <stdio.h>  // printf
#include <stdlib.h> // free

int	pipe_exec(t_ast *self, t_exec_ctx *ctx)
{
	int	lrc;
	int	rrc;

	printf("[exec] PIPE     : (left | right)\n");
	lrc = ast_exec(self->as.bin.left, ctx);
	rrc = ast_exec(self->as.bin.right, ctx);
	(void)lrc; /* パイプの戻りは一般に右側 */
	ctx->last_status = rrc;
	printf("[ret ] PIPE     : rc=%d (right)\n", rrc);
	return (rrc);
}

void	pipe_destroy(t_ast *self)
{
	printf("[free] PIPE\n");
	ast_destroy(self->as.bin.left);
	ast_destroy(self->as.bin.right);
	free(self);
}