/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operate_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:46:09 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/26 16:46:32 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void destroy_ast(t_node *n)
{
    size_t i;

    if (!n)
        return;
    if (n->type == ND_COMMAND)
    {
        free(n->as.cmd.redirs);
        free(n->as.cmd.argv_tokens);
        if (n->as.cmd.argv) 
        {
            i = 0;
            while (i < n->as.cmd.argc) 
                free(n->as.cmd.argv[i++]);
            free(n->as.cmd.argv);
        }
    }
    else if (n->type == ND_PIPE || n->type == ND_AND_IF || n->type == ND_OR_IF)
    {
        destroy_ast(n->as.bin.left);
        destroy_ast(n->as.bin.right);
    }
    else if (n->type == ND_SUBSHELL)
        destroy_ast(n->as.subshell.body);
    free(n);
}
