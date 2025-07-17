/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:30:35 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:30:36 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin_command(char *cmd)
{
	char	*builtins[8];
	int		i;

	if (!cmd)
		return (0);
	builtins[0] = "cd";
	builtins[1] = "echo";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = NULL;
	i = -1;
	while (++i < 7)
	{
		if (ft_strcmp(builtins[i], cmd) == 0)
			return (1);
	}
	return (0);
}

int	execute_builtin(t_cmd_ctx *cmd_ctx)
{
	char	*command;

	command = cmd_ctx->current->args[0];
	if (ft_strcmp(command, "cd") == 0)
		return (builtin_cd(cmd_ctx->current, cmd_ctx->env_list));
	else if (ft_strcmp(command, "echo") == 0)
		return (builtin_echo(cmd_ctx->current));
	else if (ft_strcmp(command, "pwd") == 0)
		return (builtin_pwd(cmd_ctx->env_list));
	else if (ft_strcmp(command, "export") == 0)
		return (builtin_export(cmd_ctx->current, cmd_ctx->env_list));
	else if (ft_strcmp(command, "unset") == 0)
		return (builtin_unset(cmd_ctx->current, cmd_ctx->env_list));
	else if (ft_strcmp(command, "env") == 0)
		return (builtin_env(cmd_ctx->current, *cmd_ctx->env_list));
	else if (ft_strcmp(command, "exit") == 0)
		return (builtin_exit(cmd_ctx));
	return (1);
}

int	is_parent_builtin(char *cmd)
{
	return (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "exit") == 0);
}

int	execute_single_parent_builtin(t_cmd_ctx *cmd_ctx)
{
	int		status;
	char	**envp;

	envp = env_list_to_envp(*(cmd_ctx->env_list));
	if (!envp)
		return (1);
	expand_command_args(cmd_ctx->current, envp);
	safe_doube_star_free(envp);
	status = execute_builtin(cmd_ctx);
	if (!ft_strcmp(cmd_ctx->current->args[0], "exit") && status == 2)
		exit(status);
	return (status);
}
