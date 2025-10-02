/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_bi_cd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkuwahat <tkuwahat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 20:51:34 by tkuwahat          #+#    #+#             */
/*   Updated: 2025/10/02 14:05:50 by tkuwahat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	resolve_cd_target(char *const *av, const char **dst, int *print_after)
{
	const char	*val;

	*print_after = 0;
	if (!av || !av[0] || !av[1] || ft_strcmp(av[1], "~") == 0)
	{
		val = search_env_table("HOME");
		if (!val)
			return (err3("minishell: cd: ", "HOME", " not set\n"), 1);
		*dst = val;
		return (0);
	}
	if (ft_strcmp(av[1], "-") == 0)
	{
		val = search_env_table("OLDPWD");
		if (!val)
			return (err3("minishell: cd: ", "OLDPWD", " not set\n"), 1);
		*print_after = 1;
		*dst = val;
		return (0);
	}
	*dst = av[1];
	return (0);
}

static void	update_oldpwd_if_has(const char *cwd)
{
	if (cwd && *cwd)
		(void)update_env_table("OLDPWD", cwd);
}

static void	update_pwd_from_getcwd(void)
{
	char	buf[PATH_MAX];

	if (getcwd(buf, sizeof(buf)))
		(void)update_env_table("PWD", buf);
}

static int	has_too_many_args(char **av)
{
	return (av && av[1] && av[2]);
}

int	bi_cd(char **av)
{
	char		cwd[PATH_MAX];
	const char	*dst;
	int			print_after;

	if (has_too_many_args(av))
		return (err3("minishell: cd: ", NULL, "too many arguments\n"), 1);
	dst = NULL;
	if (resolve_cd_target(av, &dst, &print_after) != 0)
		return (1);
	if (!getcwd(cwd, sizeof(cwd)))
		cwd[0] = '\0';
	if (chdir(dst) < 0)
		return (perror("minishell: cd"), 1);
	update_oldpwd_if_has(cwd);
	update_pwd_from_getcwd();
	if (print_after && getcwd(cwd, sizeof(cwd)))
		(void)print_line(STDOUT_FILENO, cwd);
	return (0);
}
