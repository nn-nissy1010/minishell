/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 22:20:02 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/16 23:50:20 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H
# include "stdlib.h"

typedef enum e_tokentype	t_tokentype;

typedef struct s_node		t_node;

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
	int						hdoc_fd;
	t_tokentype				kind;
	t_token					*word;
	int						quoted_heredoc;
	char					*path;
}							t_redir;

typedef struct s_cmd
{
	t_redir					*redirs;
	size_t					n_redirs;
	t_token					**argv_tokens;
	size_t					n_argv_tokens;
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
			struct s_node	*left;
			struct s_node	*right;
		} s_bin;
		t_cmd				cmd;
		struct
		{
			struct s_node	*body;
		} s_subshell;
	} u_as;
}							t_node;

typedef struct s_probe_flags
{
	int						found_dir;
	int						found_noexec;

}							t_probe_flags;

typedef enum e_pipe_role
{
	PIPE_LEFT,
	PIPE_RIGHT
}							t_pipe_role;

/* ast_exec / ast_destroy */
int							ast_execute_root(t_node *root, t_exec_ctx *ctx);
int							ast_exec(t_node *node, t_exec_ctx *ctx);
void						ast_destroy(t_node *node);
/* exec / destroy */
int							exec_cmd(t_node *node, t_exec_ctx *ctx);
void						destroy_cmd_min(t_node *node);
void						destroy_cmd_min_cmd(t_cmd *cmd);
void						destroy_subshell_node(struct s_node *node);
int							exec_subshell_node(struct s_node *node,
								t_exec_ctx *parent_ctx);

/* 各ノード用の関数テーブル getter */
t_func						*v_cmd(void);
t_func						*v_pipe(void);
t_func						*v_or_if(void);
t_func						*v_and_if(void);
t_func						*v_subshell(void);
#endif
