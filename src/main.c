/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:11:46 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/18 19:49:30 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_table g_env;

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
    (void)argv;
	if (env_table_init(&g_env, 128) == -1)
		return(print_syntax_error("env alloc error"), 1); 
	if (env_table_load_envp(&g_env, envp) == -1)
		return(print_syntax_error("env load error"), destroy_env_table(&g_env), 1);
	
	t_token *head = NULL;
    t_token *tail = NULL;

    // 普通のトークン作成
    t_token *a = new_token(TOK_ARG, "echo");
    t_token *b = new_token(TOK_PIPE, "|");
    t_token *c = new_token(TOK_ARG, "hello");

    // 部分文字列からトークン作成
    const char *src = "abcdef";
    t_token *d = new_token_n(TOK_ARG, src, 3); // "abc"
    t_token *e = new_token_n(TOK_ARG, src + 3, 3); // "def"

    // リストに追加
    tok_push(&head, &tail, a);
    tok_push(&head, &tail, b);
    tok_push(&head, &tail, c);
    tok_push(&head, &tail, d);
    tok_push(&head, &tail, e);

    // ダンプ出力
    for (t_token *p = head; p; p = p->next)
    {
        if (p->type == TOK_ARG)
            printf("ARG: raw=\"%s\"\n", p->u.arg.raw);
        else
            printf("SYM: type=%d\n", p->type);
    }

    // 後始末
    free_tokens(head);
    return 0;
	
	repl();
	return (0);
}
