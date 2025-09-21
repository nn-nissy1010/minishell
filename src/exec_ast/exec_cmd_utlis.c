
#include "./../../includes/exec.h"
#include "./../../includes/lexer.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static const char	*env_get(const char *name)
{
	const char	*v = getenv(name);

	return (v ? v : "");
}

static size_t	read_varname(const char *s, size_t i, size_t *len)
{
	size_t	j;

	j = i;
	if (!(isalpha((unsigned char)s[j]) || s[j] == '_'))
	{
		*len = 0;
		return (i);
	}
	j++;
	while (isalnum((unsigned char)s[j]) || s[j] == '_')
		j++;
	*len = j - i;
	return (j);
}

static int	sb_init(t_strbuf *b)
{
	b->cap = 64;
	b->len = 0;
	b->p = malloc(b->cap);
	return (b->p ? 0 : -1);
}
static void	sb_dispose(t_strbuf *b)
{
	free(b->p);
	b->p = NULL;
	b->len = b->cap = 0;
}
static int	sb_reserve(t_strbuf *b, size_t add)
{
	size_t	nc;
	char	*t;

	if (b->len + add + 1 <= b->cap)
		return (0);
	nc = b->cap;
	while (b->len + add + 1 > nc)
		nc *= 2;
	t = realloc(b->p, nc);
	if (!t)
		return (-1);
	b->p = t;
	b->cap = nc;
	return (0);
}
static int	sb_putn(t_strbuf *b, const char *s, size_t n)
{
	if (sb_reserve(b, n) != 0)
		return (-1);
	memcpy(b->p + b->len, s, n);
	b->len += n;
	return (0);
}
static int	sb_putc(t_strbuf *b, char c)
{
	if (sb_reserve(b, 1) != 0)
		return (-1);
	b->p[b->len++] = c;
	return (0);
}

/* --------- サブ処理：クォートと $処理 --------- */
/* ====== クォート/ドル処理 ====== */
static int	handle_quote(char c, int *in_sq, int *in_dq)
{
	if (c == '\'' && !*in_dq)
	{
		*in_sq = !*in_sq;
		return (1);
	}
	if (c == '"' && !*in_sq)
	{
		*in_dq = !*in_dq;
		return (1);
	}
	return (0);
}

/* $VAR → 値。処理したら1、未処理0、エラー-1。 *i は消費分だけ進める */
static int	handle_dollar(const char *in, size_t *i, t_strbuf *b, int in_sq)
{
	size_t		n;
	size_t		j;
	char		name[256];
	const char	*val;

	if (in[*i] != '$' || in_sq)
		return (0);
	n = 0;
	j = read_varname(in, *i + 1, &n);
	if (n == 0)
		return (0);
	if (n >= 256)
		return (-1);
	memcpy(name, in + *i + 1, n);
	name[n] = '\0';
	val = env_get(name);
	if (sb_putn(b, val, strlen(val)) != 0)
		return (-1);
	*i = j; /* $VAR を読み飛ばした位置へ */
	return (1);
}

/* 1文字分の展開ステップ。OK→1、継続扱い→1、エラー→-1 */
static int	step_expand(const char *in, size_t *i, t_strbuf *b, int *in_sq,
		int *in_dq)
{
	char	c;
	int		r;

	c = in[*i];
	if (handle_quote(c, in_sq, in_dq))
	{
		(*i)++;
		return (1);
	}
	r = handle_dollar(in, i, b, *in_sq);
	if (r < 0)
		return (-1);
	if (r > 0)
		return (1);
	if (sb_putc(b, c) != 0)
		return (-1);
	(*i)++;
	return (1);
}

int	expand_vars_basic(const char *in, char **out)
{
	t_strbuf	b;
	size_t		i;
	int			in_sq;
	int			in_dq;

	i = 0;
	in_sq = 0;
	in_dq = 0;
	if (sb_init(&b) != 0)
		return (-1);
	while (in[i])
		if (step_expand(in, &i, &b, &in_sq, &in_dq) < 0)
		{
			sb_dispose(&b);
			return (-1);
		}
	if (sb_putc(&b, '\0') != 0)
	{
		sb_dispose(&b);
		return (-1);
	}
	*out = b.p;
	return (0);
}

int	expand_token(const t_token *tok, char ***out_items, size_t *out_n)
{
	char	*expanded;
	char	**v;

	if (!tok || tok->type != TOK_ARG || !tok->u.arg.raw)
	{
		*out_items = NULL;
		*out_n = 0;
		return (0);
	}
	if (expand_vars_basic(tok->u.arg.raw, &expanded) != 0)
		return (-1);
	v = malloc(2 * sizeof *v);
	if (!v)
	{
		free(expanded);
		return (-1);
	}
	v[0] = expanded;
	v[1] = NULL;
	*out_items = v;
	*out_n = 1;
	return (0);
}

/* リダイレクト右辺：expand_tokenの結果が1語であることを強制 */
int	expand_redir_target(const t_token *tok, char **out_path)
{
	char	**items;
	size_t	n;

	items = NULL;
	n = 0;
	if (expand_token(tok, &items, &n) != 0)
		return (-1);
	if (n != 1)
	{
		if (items)
		{
			for (size_t i = 0; i < n; i++)
				free(items[i]);
			free(items);
		}
		return (-2); /* ambiguous */
	}
	*out_path = items[0];
	free(items);
	return (0);
}

static void	argvbuf_dispose(t_argvbuf *b)
{
	size_t	i;

	if (!b || !b->v)
		return ;
	i = 0;
	while (i < b->n)
	{
		free(b->v[i]);
		i++;
	}
	free(b->v);
	b->v = NULL;
	b->n = 0;
}

static void	free_items(char **a, size_t n)
{
	size_t	i;

	if (!a)
		return ;
	i = 0;
	while (i < n)
	{
		free(a[i]);
		i++;
	}
	free(a);
}

static int	argvbuf_append(t_argvbuf *b, char **items, size_t n)
{
	char	**nv;
	size_t	i;

	if (n == 0)
	{
		free(items);
		return (0);
	}
	nv = realloc(b->v, (b->n + n + 1) * sizeof(char *));
	if (!nv)
	{
		free_items(items, n);
		return (-1);
	}
	b->v = nv;
	i = 0;
	while (i < n)
	{
		b->v[b->n + i] = items[i];
		i++;
	}
	b->n += n;
	b->v[b->n] = NULL;
	free(items);
	return (0);
}

static int	expand_one(const t_token *t, t_argvbuf *b)
{
	char	**items;
	size_t	n;

	items = NULL;
	n = 0;
	if (expand_token(t, &items, &n) != 0)
	{
		free_items(items, n);
		return (-1);
	}
	return (argvbuf_append(b, items, n));
}

static int	ensure_nonnull(t_argvbuf *b)
{
	if (b->v)
		return (0);
	b->v = calloc(1, sizeof *b->v);
	return (b->v ? 0 : -1);
}

int	build_argv_from_tokens(const t_token *argv_toks, char ***out_argv)
{
	t_argvbuf		buf;
	const t_token	*t;

	t = argv_toks;
	memset(&buf, 0, sizeof buf);
	*out_argv = NULL;
	while (t != NULL)
	{
		if (t->type != TOK_ARG)
			break ;
		if (expand_one(t, &buf) != 0)
		{
			argvbuf_dispose(&buf);
			return (-1);
		}
		t = t->next;
	}
	if (ensure_nonnull(&buf) != 0)
	{
		argvbuf_dispose(&buf);
		return (-1);
	}
	*out_argv = buf.v;
	return (0);
}
