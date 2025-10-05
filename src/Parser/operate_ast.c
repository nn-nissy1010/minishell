/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operate_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:46:09 by nnishiya          #+#    #+#             */
/*   Updated: 2025/10/05 21:44:25 by nnishiya         ###   ########.fr       */
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

t_node **ast_ref(void)
{
	static t_node *ast = NULL;
	return (&ast);
}

void set_ast(t_node *new_ast)
{
	*ast_ref() = new_ast;
}

t_node *get_ast(void)
{
    return (*ast_ref());
}
