/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_bi_export.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnishiya <nnishiya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:02:07 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/13 11:20:56 by nnishiya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	export_set_with_value(char *arg)
{
	char	*eq;
	int		rc;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (1);
	*eq = '\0';
	if (!is_valid_ident(arg))
	{
		err3("minishell: export: `", arg, "': not a valid identifier\n");
		rc = 1;
	}
	else
	{
		if (update_env_table(arg, eq + 1) != 0)
		{
			err3("minishell: export: `", arg, "': update failed\n");
			rc = 1;
		}
		else
			rc = 0;
	}
	*eq = '=';
	return (rc);
}

static int	export_set_name_only(const char *name)
{
	const char	*cur;

	if (!is_valid_ident(name))
	{
		err3("minishell: export: `", name, "': not a valid identifier\n");
		return (1);
	}
	cur = search_env_table(name);
	if (cur != NULL)
		return (0);
	if (update_env_table(name, "") != 0)
	{
		err3("minishell: export: `", name, "': create failed\n");
		return (1);
	}
	return (0);
}

static int	export_handle_arg(char *arg)
{
	if (!arg || !*arg)
		return (0);
	if (ft_strchr(arg, '='))
		return (export_set_with_value(arg));
	return (export_set_name_only(arg));
}

static void	print_export_line(const char *key, const char *val)
{
	write(STDOUT_FILENO, "declare -x ", 11);
	write(STDOUT_FILENO, key, ft_strlen(key));
	if (val)
	{
		write(STDOUT_FILENO, "=\"", 2);
		write(STDOUT_FILENO, val, ft_strlen(val));
		write(STDOUT_FILENO, "\"", 1);
	}
	write(STDOUT_FILENO, "\n", 1);
}

void	bi_export_display(void)
{
	size_t		i;
	t_env_pair	*pair;
	t_env_table	*table;

	table = env_table();
	if (!table || !table->pair)
		return ;
	i = 0;
	while (i < table->cap)
	{
		pair = &table->pair[i];
		if (pair->key && !pair->tomb && ft_strcmp(pair->key, "_") != 0)
			print_export_line(pair->key, pair->val);
		i++;
	}
}

int	bi_export(char **av)
{
	int		i;
	int		rc;
	char	*eq;

	rc = 0;
	if (!av)
		return (0);
	if (!av[1])
	{
		bi_export_display();
		return (0);
	}
	eq = ft_strchr(av[1], '=');
	if (eq)
	{
		i = 1;
		rc = 0;
		while (av[i])
		{
			if (export_handle_arg(av[i]) != 0)
				rc = 1;
			i++;
		}
	}
	else
	{
		if (!search_env_table(av[1]))
		{
			update_env_table(av[1], NULL);
		}
	}
	return (rc);
}
