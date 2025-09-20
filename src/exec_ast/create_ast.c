
#include "./../../includes/exec.h"
#include <stdio.h>
#include <stdlib.h>

/* vtable getters */
const t_func	*v_cmd(void)
{
	static const t_func	f = {cmd_exec, cmd_destroy};

	return (&f);
}
const t_func	*v_pipe(void)
{
	static const t_func	f = {pipe_exec, pipe_destroy};

	return (&f);
}
const t_func	*v_and(void)
{
	static const t_func	f = {and_exec, and_destroy};

	return (&f);
}
const t_func	*v_or(void)
{
	static const t_func	f = {or_exec, or_destroy};

	return (&f);
}
const t_func	*v_subshell(void)
{
	static const t_func	f = {subshell_exec, subshell_destroy};

	return (&f);
}

/*================= 簡易ノード作成ヘルパ（デモ用） =================*/

t_ast	*make_cmd_node(const char *label, int rc)
{
	t_ast	*n;

	n = calloc(1, sizeof(*n));
	n->type = ND_COMMAND;
	n->fn = v_cmd();
	n->as.cmd.label = label;
	n->as.cmd.rc = rc;
	n->as.cmd.redirs = NULL;
	n->as.cmd.n_redirs = 0;
	return (n);
}

t_ast	*make_pipe_node(t_ast *l, t_ast *r)
{
	t_ast	*n;

	n = calloc(1, sizeof(*n));
	n->type = ND_PIPE;
	n->fn = v_pipe();
	n->as.bin.left = l;
	n->as.bin.right = r;
	return (n);
}

t_ast	*make_and_node(t_ast *l, t_ast *r)
{
	t_ast	*n;

	n = calloc(1, sizeof(*n));
	n->type = ND_AND_IF;
	n->fn = v_and();
	n->as.bin.left = l;
	n->as.bin.right = r;
	return (n);
}

t_ast	*make_or_node(t_ast *l, t_ast *r)
{
	t_ast	*n;

	n = calloc(1, sizeof(*n));
	n->type = ND_OR_IF;
	n->fn = v_or();
	n->as.bin.left = l;
	n->as.bin.right = r;
	return (n);
}

t_ast	*make_subshell_node(t_ast *body)
{
	t_ast *n = calloc(1, sizeof(*n));
	n->type = ND_SUBSHELL;
	n->fn = v_subshell();
	n->as.subshell.body = body;
	return (n);
}