/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:32:32 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:32:33 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	process_exit_arg(char *arg)
{
	int	exit_status;

	if (!is_number(arg))
	{
		ft_fprintf_fd(2, "minishell: exit: %s: numeric argument required\n",
			arg);
		return (2);
	}
	exit_status = ft_atoi(arg);
	return (exit_status);
}

int	builtin_exit(t_cmd_ctx *cmd_ctx)
{
	int	exit_status;

	if (cmd_ctx->cmd_list == cmd_ctx->current && cmd_ctx->current->next == NULL
		&& cmd_ctx->current->args_count == 1)
		return (ft_putstr_fd("exit\n", 1), exit(g_last_exit_status), 6789);
	if (cmd_ctx->current->args_count > 1)
	{
		exit_status = process_exit_arg(cmd_ctx->current->args[1]);
		if (exit_status == 2)
			return (exit_status);
		else if (exit_status < 0)
			return (156);
	}
	else
		exit_status = 0;
	if (cmd_ctx->current->args_count > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	free_command_list(cmd_ctx->cmd_list);
	free_env_list(cmd_ctx->env_list);
	clear_history();
	exit(exit_status);
}
