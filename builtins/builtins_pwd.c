/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:32:41 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:32:42 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_pwd(t_env **env)
{
	char	current_dir[PATH_MAX];
	t_env	*pwd_node;
	char	*pwd_env;

	if (getcwd(current_dir, sizeof(current_dir)) != NULL)
	{
		ft_fprintf_fd(1, "%s\n", current_dir);
		return (0);
	}
	else
	{
		pwd_node = find_env_node(*env, "PWD");
		if (pwd_node && pwd_node->value)
		{
			pwd_env = pwd_node->value;
			ft_fprintf_fd(1, "%s\n", pwd_env);
			return (0);
		}
	}
	ft_fprintf_fd(2, "minishell: pwd: error retrieving current directory\n");
	return (1);
}
