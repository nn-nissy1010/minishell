#include "./../../includes/minishell.h"

static void	*xmalloc(size_t n)
{
	void	*p;

	p = malloc(n);
	if (!p)
	{
		perror("malloc");
		exit(1);
	}
	return (p);
}
static char	*xstrdup(const char *s)
{
	size_t	n;
	char	*p;

	n = strlen(s) + 1;
	p = xmalloc(n);
	memcpy(p, s, n);
	return (p);
}

static t_token	*tok_arg_new(const char *raw)
{
	t_token	*t;

	t = (t_token *)xmalloc(sizeof(*t));
	t->type = TOK_ARG;
	t->next = NULL;
	t->u.arg.raw = xstrdup(raw);
	t->u.arg.parts = NULL;
	t->u.arg.items = NULL;
	t->u.arg.n_items = 0;
	t->u.arg.expanded = 0;
	return (t);
}

static void	tok_push_back(t_token **head, t_token *t)
{
	t_token	*c;

	if (!*head)
	{
		*head = t;
		return ;
	}
	c = *head;
	while (c->next)
		c = c->next;
	c->next = t;
}
void	free_token_list(t_token *t)
{
	t_token	*nx;

	while (t)
	{
		nx = t->next;
		if (t->type == TOK_ARG)
		{
			free(t->u.arg.raw);
		}
		free(t);
		t = nx;
	}
}

t_node	*make_single_cmd_node(const char *arg0, ...)
{
	t_node *n;
	va_list ap;
	const char *s = arg0;
	static const t_func vf = {exec_cmd, destroy_cmd_min};

	n = (t_node *)xmalloc(sizeof(*n));
	n->type = ND_COMMAND;
	n->fn = &vf;
	n->as.cmd.redirs = NULL;
	n->as.cmd.n_redirs = 0;
	n->as.cmd.argv_tokens = NULL;
	n->as.cmd.n_argv_tokens = 0;
	n->as.cmd.argv = NULL;
	n->as.cmd.argc = 0;
	va_start(ap, arg0);
	while (s)
	{
		tok_push_back(&n->as.cmd.argv_tokens, tok_arg_new(s));
		n->as.cmd.n_argv_tokens++;
		s = va_arg(ap, const char *);
	}
	va_end(ap);
	return (n);
}