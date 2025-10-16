/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_bi_export.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:02:07 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/16 19:00:41 by tkuwahat         ###   ########.fr       */
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

int	bi_export(char **av)
{
	int		i;
	int		rc;
	char	*eq;

	rc = 0;
	if (!av)
		return (0);
	if (!av[1])
		return (bi_export_display(), 0);
	i = 1;
	while (av[i])
	{
		eq = ft_strchr(av[i], '=');
		if (eq)
		{
			if (export_handle_arg(av[i]) != 0)
				rc = 1;
		}
		else if (!search_env_table(av[i]))
			update_env_table(av[i], NULL);
		i++;
	}
	return (rc);
}
