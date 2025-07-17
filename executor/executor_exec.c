/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:30:41 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 19:05:18 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_directory_error(char *path, char **envp)
{
	struct stat	file_stat;

	if (stat(path, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
	{
		ft_fprintf_fd(2, "minishell: %s: Command not found\n", path);
		if (ft_strchr(path, '/'))
			safe_free((void **)&path);
		safe_doube_star_free(envp);
		exit(126);
	}
	return (0);
}

static void	handle_directory_errors(char *path, char **envp)
{
	struct stat	file_stat;

	if (ft_strchr(path, '/'))
	{
		if (stat(path, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
		{
			ft_fprintf_fd(2, "minishell: %s: Is a directory\n", path);
			safe_doube_star_free(envp);
			exit(126);
		}
	}
	check_directory_error(path, envp);
	if (stat(path, &file_stat) == 0 && access(path, X_OK) == -1)
	{
		ft_fprintf_fd(2, "minishell: %s: Permission denied\n", path);
		safe_doube_star_free(envp);
		exit(126);
	}
}

static void	exec_command(char *path, t_command *current, char **envp)
{
	if (path)
	{
		check_directory_error(current->args[0], envp);
		if (execve(path, current->args, envp) == -1)
		{
			perror("minishell: execve");
			safe_free((void **)&path);
			safe_doube_star_free(envp);
			exit(126);
		}
	}
}

void	handle_external_command(t_cmd_ctx *cmd_ctx)
{
	char	*path;
	char	**envp;

	envp = env_list_to_envp(*(cmd_ctx->env_list));
	if (!envp)
		exit(1);
	if (cmd_ctx->current->args[0] && ft_strchr(cmd_ctx->current->args[0], '/'))
		handle_directory_errors(cmd_ctx->current->args[0], envp);
	path = find_executable_path(cmd_ctx->current->args[0], envp);
	exec_command(path, cmd_ctx->current, envp);
	safe_doube_star_free(envp);
	if (is_path_with_slash(cmd_ctx->current->args[0]))
	{
		ft_fprintf_fd(2, "minishell: %s", cmd_ctx->current->args[0]);
		ft_fprintf_fd(2, ": No such file or directory\n");
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_ctx->current->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	if (path)
		safe_free((void **)&path);
	exit(127);
}

void	execute_single_command(t_cmd_ctx *cmd_ctx)
{
	if (cmd_ctx->current->args && cmd_ctx->current->args[0]
		&& cmd_ctx->current->args[0][0])
	{
		if (is_builtin_command(cmd_ctx->current->args[0]))
			exit(execute_builtin(cmd_ctx));
		else
			handle_external_command(cmd_ctx);
	}
	else if (cmd_ctx->current->args && cmd_ctx->current->args[0])
	{
		ft_fprintf_fd(2, "minishell: Command '' not found\n");
		exit(127);
	}
	else
	{
		if (cmd_ctx->cmd_list->redirections)
			exit(0);
		exit(0);
	}
}
