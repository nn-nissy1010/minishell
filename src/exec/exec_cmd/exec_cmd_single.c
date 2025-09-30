/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_single.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 20:33:07 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/30 13:06:33 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_strtok_r(char *s, const char *delim, char **saveptr)
{
    char *start;
    char *end;

    if (!s)
        s = *saveptr;
    if (!s)
        return NULL;

    while (*s && ft_strchr(delim, *s))
        s++;
    if (*s == '\0')
        return NULL;

    start = s;
    while (*s && !ft_strchr(delim, *s))
        s++;

    if (*s) {
        *s = '\0';
        s++;
    }
    *saveptr = s;
    return start;
}



/* ==== child-only error printing (single write) ==== */
static void err3(const char *a, const char *b, const char *c)
{
    char   buf[512];
    size_t n;

    buf[0] = '\0';
    n = 0;
    n += ft_strlcpy(buf + n, a ? a : "", sizeof(buf) - n);
    n += ft_strlcpy(buf + n, b ? b : "", sizeof(buf) - n);
    n += ft_strlcpy(buf + n, c ? c : "", sizeof(buf) - n);
    if (n > sizeof(buf)) n = sizeof(buf);
    (void)write(STDERR_FILENO, buf, n);
}

static void err3(const char *prefix, const char *subject, const char *message)
{
    char   buf[512];
    size_t n;

    buf[0] = '\0';
    n = 0;
    if (prefix)
        n += ft_strlcpy(buf + n, prefix, sizeof(buf) - n);
    else
        n += ft_strlcpy(buf + n, "", sizeof(buf) - n);
    if (subject)
        n += ft_strlcpy(buf + n, subject, sizeof(buf) - n);
    else
        n += ft_strlcpy(buf + n, "", sizeof(buf) - n);
    if (message)
        n += ft_strlcpy(buf + n, message, sizeof(buf) - n);
    else
        n += ft_strlcpy(buf + n, "", sizeof(buf) - n);
    if (n > sizeof(buf))
        n = sizeof(buf);
    (void)write(STDERR_FILENO, buf, n);
}


static int is_directory(const char *path)
{
    struct stat st;

    if (!path) 
		return 0;
    if (stat(path, &st) < 0) 
		return 0;
     if (S_ISDIR(st.st_mode))
        return 1;
	return  0;
}

static char *path_join(const char *dir, const char *cmd)
{
    size_t dl; 
	size_t cl;
    char  *p;

    dl = ft_strlen(dir);
    cl = ft_strlen(cmd);
    p = (char *)malloc(dl + 1 + cl + 1);
    if (!p) 
		return NULL;
    ft_memcpy(p, dir, dl);
    p[dl] = '/';
    ft_memcpy(p + dl + 1, cmd, cl);
    p[dl + 1 + cl] = '\0';
    return p;
}

/* ==== 子側：絶対/相対パスをそのまま実行 ==== */
static void exec_direct(char **av, char **envp)
{
    if (is_directory(av[0])) 
	{
        err3("minishell: ", av[0], ": is a directory\n");
        exit(126);
    }
    execve(av[0], av, envp);
    if (errno == ENOENT) 
	{
        err3("minishell: ", av[0], ": No such file or directory\n");
        exit(127);
    }
    if (errno == EACCES) 
	{
        err3("minishell: ", av[0], ": Permission denied\n");
        exit(126);
    }
    err3("minishell: ", av[0], ": exec error\n");
    exit(126);
}

/* ==== 子側：PATH を探索して実行 ==== */
static void search_and_exec(char **av, char **envp)
{
    const char *path;
    char       *dup;
    char       *save;
    char       *dir;
    char       *full;

    path = search_env_table("PATH");
    if (!path || *path == '\0') 
	{
        err3("minishell: ", av[0], ": command not found\n");
        exit(127);
    }
    dup = ft_strdup(path);
    if (!dup) 
	{
        err3("minishell: ", "malloc", ": failed\n");
        exit(126);
    }
    save = dup;
    while ((dir = ft_strtok_r(dup, ":", &dup))) 
	{
        if (*dir == '\0') dir = ".";
        full = path_join(dir, av[0]);
        if (full) 
		{
            if (access(full, X_OK) == 0 && !is_directory(full))
                execve(full, av, envp); 
            free(full);
        }
    }
    free(save);
    err3("minishell: ", av[0], ": command not found\n");
    exit(127);
}



static void set_child_signals_default(void)
{
    struct sigaction sa;

    ft_memset(&sa, 0, sizeof(sa));
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_DFL;
    x_sigaction(SIGINT,  &sa);
    x_sigaction(SIGQUIT, &sa);
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
