
// #include "../../includes/minishell.h"

#include "./../../includes/exec.h"
#include <stdio.h>  // printf
#include <stdlib.h> // free

/*================= 実行関数（中身はprintfのみ） =================*/

int	cmd_exec(t_ast *self, t_exec_ctx *ctx)
{
	const t_cmd	*c = &self->as.cmd;

	printf("[exec] CMD      : \"%s\" -> rc=%d\n",
		c->label ? c->label : "(null)", c->rc);
	ctx->last_status = c->rc;
	return (c->rc);
}

void	cmd_destroy(t_ast *self)
{
	/* redirs 配列だけ解放（wordの所有権はプロジェクト方針に合わせて） */
	free(self->as.cmd.redirs);
	printf("[free] CMD      : \"%s\"\n",
		self->as.cmd.label ? self->as.cmd.label : "(null)");
	free(self);
}