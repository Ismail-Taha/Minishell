/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:32:29 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:32:30 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_env_var_format(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	if (i > 0 && str[i] == '=')
		return (1);
	return (0);
}

static int	process_env_args(t_command *cmd, int *first_cmd_arg)
{
	int	extra_vars;

	*first_cmd_arg = 1;
	extra_vars = 0;
	while (*first_cmd_arg < cmd->args_count
		&& is_env_var_format(cmd->args[*first_cmd_arg]))
	{
		(*first_cmd_arg)++;
		extra_vars++;
	}
	return (extra_vars);
}

static char	**setup_and_copy_env(t_env *env_list, t_command *cmd,
		int *first_cmd_arg)
{
	t_env_setup	env;

	env.extra_vars = process_env_args(cmd, first_cmd_arg);
	env.env_array = env_list_to_envp(env_list);
	if (!env.env_array)
		return (NULL);
	env.env_count = 0;
	while (env.env_array[env.env_count])
		env.env_count++;
	env.temp_env = ft_calloc(env.env_count + env.extra_vars + 1,
			sizeof(char *));
	if (!env.temp_env)
		return (safe_doube_star_free(env.env_array), NULL);
	env.i = -1;
	while (env.env_array[++env.i])
		env.temp_env[env.i] = ft_strdup(env.env_array[env.i]);
	env.j = 1;
	while (env.j < *first_cmd_arg)
		env.temp_env[env.i++] = ft_strdup(cmd->args[env.j++]);
	safe_doube_star_free(env.env_array);
	return (env.temp_env);
}

static int	handle_cmd_exec(t_command *cmd, t_env *env_list, int first_cmd_arg)
{
	char	**temp_env;

	temp_env = setup_and_copy_env(env_list, cmd, &first_cmd_arg);
	if (!temp_env)
		return (1);
	if (access(cmd->args[first_cmd_arg], F_OK) == -1)
	{
		ft_fprintf_fd(2, "env: %s", cmd->args[first_cmd_arg]);
		ft_fprintf_fd(2, ": No such file or directory\n");
		safe_doube_star_free(temp_env);
		return (127);
	}
	safe_doube_star_free(temp_env);
	return (0);
}

int	builtin_env(t_command *cmd, t_env *env_list)
{
	int	first_cmd_arg;

	first_cmd_arg = 1;
	while (first_cmd_arg < cmd->args_count
		&& is_env_var_format(cmd->args[first_cmd_arg]))
		first_cmd_arg++;
	if (first_cmd_arg < cmd->args_count)
		return (handle_cmd_exec(cmd, env_list, first_cmd_arg));
	print_environment(env_list, cmd);
	return (0);
}
