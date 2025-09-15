/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:11:30 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/15 15:56:48 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

extern int g_exit_status;

typedef enum e_more {
    MORE_NONE = 0,
    MORE_QUOTE_S,
    MORE_QUOTE_D,
    MORE_OP
} t_more;

typedef struct s_env_pair {
char *key;
char *val;
int tomb;
} t_env_pair;

typedef struct s_env_table {
size_t cap;
size_t size;
t_env_pair *pair;
} t_env_table;

typedef struct s_set_ctx {
    t_env_table *t;
    size_t first_tomb;
    size_t idx;
    const char *key;
    const char *val;
}   t_set_ctx;

extern t_env_table g_env;

typedef void (*t_env_iter_cb)(const char *k, const char *v, void *ud);

int repl(void);

void install_signal_handlers(void);

char *read_full_command_line(char *first_line);

int	handle_no_quote(char c, int *q, size_t *i, const char *s);
int	handle_single_quote(char c, int *q);
int	handle_double_quote(char c, int *q, size_t i, const char *s);
t_more need_more_input(const char *s);

int ft_isspace(unsigned char c);
int is_blank_line(const char *s);
char	*ft_strjoin_3word(char *s1, const char *s2, const char *s3);
void	print_syntax_error(const char *msg);

int env_table_init(t_env_table *t, size_t cap_hint);
int env_table_load_envp(t_env_table *t, char **envp);
unsigned long fnv1a(const char *s);
int env_table_set(t_env_table *t, const char *key, const char *val);
int pair_set(t_env_pair *p, const char *k, const char *v);

void print_cb(const char *k, const char *v, void *ud);
void env_table_foreach(const t_env_table *t, t_env_iter_cb cb, void *ud);
int env_table_unset(t_env_table *t, const char *key);
void free_entry(t_env_pair *p);
int env_table_put_entry(t_env_table *t, const char *entry);

int get_env_table(void);
int update_env_table(const char *arg);
int unset_env_table(const char *arg);
void destroy_env_table(t_env_table *t);
