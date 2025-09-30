/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_apply_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 22:13:34 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/09/30 10:18:41 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void pre_backup_cleanup(int *saved_in, int *saved_out)
{
    if (*saved_in != -1) 
	{ 
		close(*saved_in);  
		*saved_in = -1; 
	}
    if (*saved_out != -1)
	{ 
		close(*saved_out); 
		*saved_out = -1; 
	}
}

static int open_for_redir(const t_redir *r)
{
	if (!r || !r->path) 
		return -1;
    if (r->kind == TOK_REDIR_IN)
        return open(r->path, O_RDONLY);
    if (r->kind == TOK_REDIR_OUT)
        return open(r->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (r->kind == TOK_REDIR_APPEND)
        return open(r->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
    return -1;
}

static int redir_prepare_src(t_redir *r, int *out_src)
{
    int fd;

    if (!r || !out_src)
        return -1;
    if (r->kind == TOK_HEREDOC) 
	{
        if (r->hdoc_fd < 0)
            return -1;
        *out_src = r->hdoc_fd;
        return 0;
    }
    fd = open_for_redir(r);
    if (fd < 0)
        return -1;
    *out_src = fd;
    return 0;
}

int apply_one_redir(t_redir *r)
{
    int target;
    int src;

    if (!r)
        return -1;
    target = target_fd(r);                
    if (redir_prepare_src(r, &src) < 0)
        return -1;
    if (src != target) {
        if (dup2(src, target) < 0) 
		{
            close(src);
            return -1;
        }
        if (close(src) < 0)
            return -1;
    }
    if (r->kind == TOK_HEREDOC)
        r->hdoc_fd = -1;                
    return 0;
}
