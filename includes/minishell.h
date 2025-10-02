/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:12:13 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/02 11:24:37 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SHELL
# define MINI_SHELL

# include <ctype.h>
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
# include "env_table.h"
# include "input.h"
# include "lexer.h"
# include "libft.h"
# include "exec.h"
# include "parser.h"

extern volatile sig_atomic_t	g_signal;

int								get_exit_status(void);
void							set_exit_status(int status);
void							reset_exit_status(void);
void							inc_exit_status(int delta);

int								ft_isspace(unsigned char c);
int								is_blank_line(const char *s);
char							*ft_strjoin_3word(char *s1, const char *s2,
									const char *s3);
void							print_syntax_error(const char *msg);
char							*ft_strndup(const char *s, size_t n);
void							*ft_realloc(void *ptr, size_t size,
									size_t old_size);

/* expand*/
int								expansion(t_cmd *c);
/* expand_arg */
int								expand_argv_cmd(t_cmd *c);
int								expand_argv(t_token **argv_tokens,
									size_t n_tokens, char ***out_argv,
									size_t *out_argc);

/* expand_redirect */
int								expand_redirs(t_redir *rs, size_t n);

/* expand_part */
char							*expand_params_part(const char *s);
int								expand_one_part_buf(t_buf *b, t_arg_part *p,
									int is_head);
int								parts_to_buf(t_arg_part *parts, t_buf *out);

int								apply_tilde_if_head_buf(t_buf *b, const char *s,
									t_quote_type q, int is_head);

/* word_split*/
int								is_unquoted_ifs_at(const t_buf *b, size_t i);
size_t							skip_unquoted_ifs(const t_buf *b, size_t i);
size_t							count_fields_from_buf(const t_buf *b);
size_t							field_span(const t_buf *b, size_t i);
int								split_fields_from_buf(const t_buf *b,
									t_arg *arg);
void							arg_clear_items(t_arg *arg);

/* expand_buf_utils*/
int								buf_putc(t_buf *b, char c);
int								buf_putn(t_buf *b, const void *p, size_t n);
int								buf_puts(t_buf *b, const char *s);
void							buf_clear(t_buf *b);
void							buf_free(t_buf *b);
void							buf_init(t_buf *b);
char							*buf_take(t_buf *b);
int								buf_take_pair(t_buf *b, char **out_data,
									unsigned char **out_qmask);
int								emit_env(t_buf *b, const char *name);
int								emit_status(t_buf *b);
int								buf_qreserve(t_buf *b, size_t add);
int								buf_puts_q(t_buf *b, const char *s,
									unsigned char q);
int								buf_putn_q(t_buf *b, const void *p, size_t n,
									unsigned char q);
int								buf_putc_q(t_buf *b, char c, unsigned char q);

/*arg_buf*/
int								argbuf_move_append(t_argbuf *dst, t_argbuf *src);
int								argbuf_terminate(t_argbuf *b);
int								argbuf_push(t_argbuf *b, const char *s);
int								argbuf_grow(t_argbuf *b, size_t newcap);
void							argbuf_free(t_argbuf *b);
void							argbuf_init(t_argbuf *b);

/*heredoc_utils*/
void							hdoc_set_signals(void);
int								hdoc_abort_sigint(int p0, int p1, char *line);
int								hdoc_should_abort(int p0, int p1);
int								hdoc_prepare(int pfd[2]);
int								hdoc_finish_success(int pfd[2], int *out_fd);
int								heredoc_build_delim_inplace(t_redir *r);

int								hdoc_read_line(char **out_line, int p0, int p1);
char							*hdoc_maybe_expand(const char *line,
									int quoted);
int								hdoc_write_line(int fd, const char *s);
int								hdoc_open_pipe(int pfd[2]);

int								collect_heredocs(t_cmd *c);

/*glob*/
int								glob_expand_argv(t_cmd *c);
int								collect_matches_in_cwd(const char *pattern, t_argbuf *out);
int								pat_match(const char *p, const char *s);
int								should_glob_expand(const char *s);
int								argbuf_move_append(t_argbuf *dst,
									t_argbuf *src);
void							unmask_globs(char *s);
int 							expand_glob_pattern(const char *pattern, t_argbuf *out);

void							sort_strptrs(char **v, size_t n);

/*exec_cmd_call*/
int								prepare_cmd_for_exec(t_cmd *c);
int								exec_single(t_cmd *c, t_exec_ctx *ctx);

int								exec_cmd(t_node *node, t_exec_ctx *ctx);
void							destroy_cmd_min(t_node *node);

/*exec_cmd_parent*/
int								run_parent_builtin_flow(t_cmd *c);
int								wait_and_status(pid_t pid);
int 							parent_finalize_simple(pid_t pid);

void							buf_cat(char *buf, size_t *pn, size_t cap, const char *s);

/*exec_cmd_redirect*/
int								redirect_only_flow(t_cmd *c);
int								apply_redirs(t_redir *r, size_t n, int *saved_in, int *saved_out);
int								apply_one_redir(t_redir *r);
int								target_fd(const t_redir *r);
void							rollback_and_invalidate(int *saved_in, int *saved_out);
void							pre_backup_cleanup(int *saved_in, int *saved_out);
void							restore_stdio(int saved_in, int saved_out);
int								pre_backup(const t_redir *r, size_t n, int *saved_in, int *saved_out);

/*exec_cmd_child*/
pid_t							spawn_child(t_cmd *c);
void							child_main_after_fork(t_cmd *c);
void							set_child_signals_default(void);
int								is_builtin_any(const t_cmd *c);
int								run_builtin_child(t_cmd *c);
void							run_external_in_child(t_cmd *c);
int								has_slash(const char *s);
void							search_and_exec(char **av);
void							probe_dir_entry(const char *dir, char **av, char **envp,t_probe_flags *f);
int								is_directory(const char *path);
void							err3(const char *prefix, const char *subject, const char *message);
void							exit_enoexec(const char *cmd);
void							finalize_path_search(const char *cmd, const t_probe_flags *f);
void							exec_direct(char **av);
void							free_env_array(char **envp);
const char						*exec_errmsg(int err);
void							fail_exec(const char *path, int err);


/*exec_cmd_builtin*/
int								bi_cd(char **av);
int								bi_echo(char **av);
int								bi_env(char **av);
int								bi_exit(char **av);
int								bi_export(char **av);
int								bi_pwd(char **av);
int								bi_unset(char **av);
int								is_valid_ident(const char *s);

#endif
