/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_single.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 20:33:07 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/30 10:35:16 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"






/* run_external_in_child.c */
#include "minishell.h"
#include <sys/stat.h>
#include <errno.h>

/* ---- small utils ---- */

static int has_slash(const char *s)
{
    size_t i;

    if (!s) return 0;
    i = 0;
    while (s[i]) { if (s[i] == '/') return 1; i++; }
    return 0;
}

static int is_directory(const char *path)
{
    struct stat st;
    if (!path) return 0;
    if (stat(path, &st) < 0) return 0;
    return S_ISDIR(st.st_mode) ? 1 : 0;
}

static void put_err3(const char *a, const char *b, const char *c)
{
    if (a) write(STDERR_FILENO, a, ft_strlen(a));
    if (b) write(STDERR_FILENO, b, ft_strlen(b));
    if (c) write(STDERR_FILENO, c, ft_strlen(c));
}

/* minishell: name: msg\n */
static void print_err(const char *name, const char *msg)
{
    put_err3("minishell: ", name, ": ");
    put_err3(msg, "\n", NULL);
}

static void print_errno(const char *name)
{
    put_err3("minishell: ", name, ": ", NULL);
    put_err3(strerror(errno), "\n", NULL);
}

static char *path_join(const char *dir, const char *cmd)
{
    size_t dl, cl;
    char   *p;

    dl = ft_strlen(dir);
    cl = ft_strlen(cmd);
    p = (char *)malloc(dl + 1 + cl + 1);
    if (!p) return NULL;
    ft_memcpy(p, dir, dl);
    p[dl] = '/';
    ft_memcpy(p + dl + 1, cmd, cl);
    p[dl + 1 + cl] = '\0';
    return p;
}

static const char *get_env(const char *const *envp, const char *key)
{
    size_t klen;
    size_t i;

    if (!envp || !key) return NULL;
    klen = ft_strlen(key);
    i = 0;
    while (envp[i]) {
        if (!ft_strncmp(envp[i], key, klen) && envp[i][klen] == '=')
            return envp[i] + klen + 1;
        i++;
    }
    return NULL;
}

/* ---- PATH search & exec ---- */

static void exec_direct(char **av, char **envp)
{
    if (is_directory(av[0])) {
        print_err(av[0], "is a directory");
        _exit(126);
    }
    execve(av[0], av, envp);
    if (errno == ENOENT) {
        print_err(av[0], "No such file or directory");
        _exit(127);
    }
    if (errno == EACCES) { print_errno(av[0]); _exit(126); }
    print_errno(av[0]);
    _exit(126);
}

static void search_and_exec(char **av, char **envp)
{
    const char *path;
    char       *dup;
    char       *save;
    char       *dir;
    char       *full;

    path = get_env((const char *const *)envp, "PATH");
    if (!path || *path == '\0') {
        print_err(av[0], "command not found");
        _exit(127);
    }
    dup = ft_strdup(path);
    if (!dup) { print_errno("malloc"); _exit(126); }
    save = dup;
    while ((dir = ft_strtok_r(dup, ":", &dup))) {
        if (*dir == '\0') dir = ".";
        full = path_join(dir, av[0]);
        if (full && access(full, X_OK) == 0) {
            if (is_directory(full)) { free(full); print_err(av[0], "is a directory"); _exit(126); }
            execve(full, av, envp);
            /* ここに来たら exec 失敗: fallthrough で次へ */
        }
        if (full) free(full);
    }
    free(save);
    print_err(av[0], "command not found");
    _exit(127);
}


static void set_child_signals_default(void)
{
    struct sigaction sa;

    ft_memset(&sa, 0, sizeof(sa));
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_DFL;
    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}



void run_external_in_child(t_cmd *c, char **envp)
{
    if (!c || !c->argv || !c->argv[0]) 
		exit(0);
    set_child_signals_default();

    if (has_slash(c->argv[0]))
        exec_direct(c->argv, envp);
    else
        search_and_exec(c->argv, envp);

		exit(127);
}






static int dup2_checked(int oldfd, int newfd)
{
    if (oldfd == newfd)
        return 0;               
    if (dup2(oldfd, newfd) < 0)
        return -1;
    return 0;
}

void restore_stdio(int saved_in, int saved_out)
{
    if (saved_in >= 0 && saved_in != STDIN_FILENO) 
	{
        (void)dup2_checked(saved_in, STDIN_FILENO);
        close(saved_in);
    }
    if (saved_out >= 0 && saved_out != STDOUT_FILENO) 
	{
        (void)dup2_checked(saved_out, STDOUT_FILENO);
        close(saved_out);
    }
}

int is_builtin_parent(const t_cmd *c)
{
	const char *b;

	if (!c)
		return (0);
	if (!c->argv)
		return (0);
	if (!c->argv[0])
		return (0);
	b = c->argv[0];
	if (ft_strcmp(b, "cd") == 0)
		return (1);
	if (ft_strcmp(b, "export") == 0)
		return (1);
	if (ft_strcmp(b, "unset") == 0)
		return (1);
	if (ft_strcmp(b, "exit") == 0)
		return (1);
	return (0);
}

int exec_single(t_cmd *c, t_exec_ctx *ctx)
{
    int   saved_in;
    int   saved_out;
    pid_t pid;
    int   status;
    int   in_sub_or_pipe;

    if (!c || !c->argv || !c->argv[0])
        return 0;
    in_sub_or_pipe = (ctx && (ctx->xflag & (XF_PIPEIN | XF_PIPEOUT | XF_SUBSHELL)));
    if (!in_sub_or_pipe && is_builtin_parent(c)) 
	{
        if (apply_redirs(c->redirs, c->n_redirs, &saved_in, &saved_out) < 0)  
            return (set_exit_status(1),-1);
        status = run_builtin_parent(c);
        restore_stdio(saved_in, saved_out);
        set_exit_status(status);
        return 0;
    }
    if (apply_redirs(c->redirs, c->n_redirs, &saved_in, &saved_out) < 0) 
	{
        set_exit_status(1);
        return -1;
    }
    pid = fork();
    if (pid < 0) 
	{
        perror("fork");
        restore_stdio(saved_in, saved_out);
        set_exit_status(1);
        return -1;
    }
    if (pid == 0)
        run_external_in_child(c, get_env_table()); 
    restore_stdio(saved_in, saved_out);
    status = wait_and_status(pid);
    set_exit_status(status);
    return 0;
}
