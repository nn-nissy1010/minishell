/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 22:20:02 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/25 12:52:14 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "stdlib.h"

typedef enum e_xflag
{
	XF_NONE = 0,
	XF_PIPEIN = 1 << 0,
	XF_PIPEOUT = 1 << 1,
	XF_SUBSHELL = 1 << 2
}							t_xflag;
typedef struct s_exec_ctx
{
	int						xflag;
}							t_exec_ctx;

struct s_node;

typedef struct s_func
{
	int						(*exec)(struct s_node *node, t_exec_ctx *ctx);
	void					(*destroy)(struct s_node *node);
}							t_func;

typedef enum e_ntype
{
	ND_COMMAND,
	ND_PIPE,
	ND_AND_IF,
	ND_OR_IF,
	ND_SUBSHELL
}							t_ntype;

typedef struct s_token		t_token;

typedef struct s_redir
{
	int						fd;
	t_token					*word;
	int						quoted_heredoc;
	char					*path;
}							t_redir;

typedef struct s_cmd
{
	t_redir					*redirs;
	size_t					n_redirs;

	/* 展開前~wordsplittingまで終え、itemsに格納 */

	t_token					**argv_tokens;
	size_t					n_argv_tokens;

	/* --- 展開後（exec直前に作る）--- */

	char					**argv;
	size_t					argc;
}							t_cmd;

typedef struct s_node
{
	const t_func			*fn;
	t_ntype					type;
	union
	{
		struct
		{
			struct s_node *left, *right;
		} bin;
		t_cmd				cmd;
		struct
		{
			struct s_node	*body;
		} subshell;
	} as;
}							t_node;

/* ast_exec / ast_destroy */
int							ast_exec(t_node *node, t_exec_ctx *ctx);
void						ast_destroy(t_node *node);
/* exec / destroy */
int							exec_cmd(t_node *node, t_exec_ctx *ctx);
void						destroy_cmd_min(t_node *node);

/* 各ノード用の関数テーブル getter */
t_func						*v_cmd(void);

#endif
