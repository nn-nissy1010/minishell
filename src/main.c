/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:11:46 by nnishiya          #+#    #+#             */
/*   Updated: 2025/09/15 13:07:46 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status = 0;
t_env_table g_env;

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
    (void)argv;
	if (env_table_init(&g_env, 128) == -1)
		return(print_syntax_error("env alloc error"), 1); 
	if (env_table_load_envp(&g_env, envp) == -1)
		return(print_syntax_error("env load error"), env_table_destroy(&g_env), 1);
	
	////環境変数確認用(ハッシュテーブルの表示)
	size_t      i;
	t_env_pair  *p;
	i = 0;
	while (i < g_env.cap)
	{
		p = &g_env.pair[i];
		if (p->key && !p->tomb)
		{
			if (p->val)
				printf("%s=%s\n", p->key, p->val);
			else
				printf("%s\n", p->key);
		}
		i++;
	}
	printf("%s\n", "----------------------ここから-----------------------");
	////環境変数確認用(envpの表示)
	i = 0;
	while (envp[i])
    {
        printf("%s\n", envp[i]);
        i++;
    }
    return 0;

	repl();
	return (0);
}
