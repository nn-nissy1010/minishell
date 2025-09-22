
#include "../../includes/minishell.h"

t_func	*v_cmd(void)
{
	static t_func	f = {exec_cmd, destroy_cmd_min};

	return (&f);
}
int	exec_cmd(t_node *node, t_exec_ctx *ctx)
{
	t_token	*tok;

	(void)ctx;
	printf("[exec_cmd] called: node->type=%d\n", node->type);
	if (node->as.cmd.argv_tokens)
	{
		tok = node->as.cmd.argv_tokens;
		printf("  argv_tokens:");
		while (tok)
		{
			if (tok->type == TOK_ARG && tok->u.arg.raw)
				printf(" %s", tok->u.arg.raw);
			tok = tok->next;
		}
		printf("\n");
	}
	return (0);
}

void	destroy_cmd_min(t_node *node)
{
	if (!node)
		return ;
	free_token_list(node->as.cmd.argv_tokens);
}
