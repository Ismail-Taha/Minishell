/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:30:21 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:30:22 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_cmd_list(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		printf("\n%d\n", cmd->args_count);
		i = 0;
		while (i < cmd->args_count)
			printf("%s ", cmd->args[i++]);
		printf("\n");
		cmd = cmd->next;
	}
}

void	free_env_list(t_env **env_list)
{
	t_env	*current;
	t_env	*next;

	if (!env_list || !*env_list)
		return ;
	current = *env_list;
	while (current)
	{
		next = current->next;
		if (current->key)
			safe_free((void **)&current->key);
		if (current->value)
			safe_free((void **)&current->value);
		safe_free((void **)&current);
		current = next;
	}
	*env_list = NULL;
}

void	update_shlvl(t_env **env_list)
{
	int		shlvl;
	char	*shlvl_str;
	t_env	*node;

	shlvl = 1;
	node = find_env_node(*env_list, "SHLVL");
	if (node && node->value)
	{
		shlvl = ft_atoi(node->value) + 1;
		if (shlvl < 0)
			shlvl = 0;
		else if (shlvl > 999)
		{
			ft_putstr_fd("minishell: warning: shell level (", 2);
			ft_putnbr_fd(shlvl, 2);
			ft_putstr_fd(") too high, resetting to 1\n", 2);
			shlvl = 1;
		}
	}
	shlvl_str = ft_itoa(shlvl);
	if (shlvl_str)
	{
		add_or_update_env(env_list, "SHLVL", shlvl_str);
		safe_free((void **)&shlvl_str);
	}
}

void	initialize_empty_env(t_env **env_list)
{
	char	cwd[PATH_MAX];
	char	*shell_path;

	add_or_update_env(env_list, "SHLVL", "1");
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		add_or_update_env(env_list, "PWD", cwd);
	shell_path = "env";
	add_or_update_env(env_list, "_", shell_path);
}
