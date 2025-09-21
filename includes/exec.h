
/* ast.h */
#ifndef EXEC_H
# define EXEC_H

# include "env_table.h"
# include "input.h"
# include "lexer.h"
# include <ctype.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_exec_ctx
{
	int last_status; /* $? */
}							t_exec_ctx;

struct s_ast;

/* ノード関数テーブル */
typedef struct s_func
{
	int						(*exec)(struct s_ast *node, t_exec_ctx *ctx);
	void					(*destroy)(struct s_ast *node);
}							t_func;

typedef enum e_ntype
{
	ND_COMMAND,
	ND_PIPE,
	ND_AND_IF,
	ND_OR_IF,
	ND_SUBSHELL
}							t_ntype;

/* ==== types.h 抜粋 ==== */
typedef enum e_redir_kind
{
	RD_IN,
	RD_OUT,
	RD_APP,
	RD_HDOC
}							t_redir_kind;

typedef struct s_token t_token; /* TOK_ARG を想定（右辺の語/デリミタ） */

typedef struct s_redir
{
	t_redir_kind			kind;
	int						fd;
	t_token					*word;
	int						quoted_heredoc;
	char					*path;
}							t_redir;

/* ←ここが更新: cmd が redirs を持つ */
typedef struct s_cmd
{
	const char *label; /* デモ表示用 */
	int rc;            /* デモ用の擬似終了コード */

	t_redir					*redirs;
	size_t					n_redirs;

	/* --- 引数（展開前）--- */
	t_token					*argv_tokens;
	size_t					n_argv_tokens;

	/* --- 展開後（exec直前に作る）--- */
	char					**argv;
	size_t					argc;

}							t_cmd;

typedef struct s_ast
{
	const t_func			*fn;
	t_ntype					type;
	union
	{
		struct
		{
			struct s_ast *left, *right;
		} bin;
		t_cmd				cmd;
		struct
		{
			struct s_ast	*body;
		} subshell;
	} as;
}							t_ast;

typedef struct s_argvbuf
{
	char					**v;
	size_t					n;
}							t_argvbuf;

typedef struct s_strbuf
{
	char					*p;
	size_t					len;
	size_t					cap;
}							t_strbuf;

/* ast_exec / ast_destroy */
int							ast_exec(t_ast *node, t_exec_ctx *ctx);
void						ast_destroy(t_ast *node);

/* 各ノード用の関数テーブル getter */
const t_func				*v_cmd(void);
const t_func				*v_pipe(void);
const t_func				*v_and(void);
const t_func				*v_or(void);
const t_func				*v_subshell(void);

int							cmd_exec(t_ast *self, t_exec_ctx *ctx);
void						cmd_destroy(t_ast *self);
int							pipe_exec(t_ast *self, t_exec_ctx *ctx);
void						pipe_destroy(t_ast *self);
int							and_exec(t_ast *self, t_exec_ctx *ctx);
void						and_destroy(t_ast *self);
int							or_exec(t_ast *self, t_exec_ctx *ctx);
void						or_destroy(t_ast *self);
int							subshell_exec(t_ast *self, t_exec_ctx *ctx);
void						subshell_destroy(t_ast *self);

t_ast						*make_cmd_node(const char *label, int rc);

t_ast						*make_pipe_node(t_ast *l, t_ast *r);
t_ast						*make_and_node(t_ast *l, t_ast *r);
t_ast						*make_or_node(t_ast *l, t_ast *r);
t_ast						*make_subshell_node(t_ast *body);

/* 1語のTK_ARGを0..N語へ展開（最小版：そのまま1語にする） */
int							expand_token(const t_token *tok, char ***out_items,
								size_t *out_n);

/* リダイレクト右辺：展開後が1語ならOK、0語/2語以上はエラー */
int							expand_redir_target(const t_token *tok,
								char **out_path);
int							build_argv_from_tokens(const t_token *argv_toks,
								char ***out_argv);

const char					*search_env_table(const char *key);

#endif
