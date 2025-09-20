

/* exec_ast.c */
#define _POSIX_C_SOURCE 200809L
#include "./../../includes/exec.h"
#include "./../../includes/lexer.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/*================= ディスパッチ =================*/
int	ast_exec(t_ast *node, t_exec_ctx *ctx)
{
	return (node->fn->exec(node, ctx));
}
void	ast_destroy(t_ast *node)
{
	node->fn->destroy(node);
}

/*================= おまけ：デモ main（任意） =================*/
/* 実案件では parser が AST を組んだ後、ast_exec(root,&ctx) を呼ぶだけでOK */

int	main(void)
{
	t_ast		*a;
	t_ast		*b;
	t_ast		*c;
	t_ast		*d;
	t_exec_ctx	ctx;
	int			rc;
	t_ast		*root;

	/* 例: ( (a && b) | (c || d) ) のフロー確認 */
	a = make_cmd_node("a", 0);
	b = make_cmd_node("b", 1);
	c = make_cmd_node("c", 0);
	d = make_cmd_node("d", 0);
	t_ast *left = make_and_node(a, b); /* a==0
-> b 実行、b==1 */
	t_ast *right = make_or_node(c, d); /* c==0
-> d は実行されない */
	root = make_pipe_node(left, make_subshell_node(right));
	/* パイプは右のrcを返す */
	ctx.env = NULL;
	ctx.last_status = 0;
	rc = ast_exec(root, &ctx);
	printf("=== FINAL rc=%d, $?=%d ===\n", rc, ctx.last_status);
	ast_destroy(root);
	return (rc);
}
