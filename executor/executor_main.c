/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:30:43 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 19:26:59 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	setup_pipes_and_heredocs(t_cmd_ctx *cmd_ctx)
{
	int		setup_result;
	char	**envp;

	cmd_ctx->pipe_fd[0] = -1;
	cmd_ctx->pipe_fd[1] = -1;
	cmd_ctx->prev_pipe_read = -1;
	envp = env_list_to_envp(*(cmd_ctx->env_list));
	setup_result = setup_all_heredocs(cmd_ctx->cmd_list, envp);
	safe_doube_star_free(envp);
	if (setup_result == 130)
	{
		g_last_exit_status = 130;
		return (130);
	}
	if (setup_result == -1)
	{
		return (1);
	}
	return (0);
}

static int	setup_command_pipe(t_cmd_ctx *cmd_ctx)
{
	if (cmd_ctx->current->next)
	{
		if (setup_pipe(cmd_ctx->pipe_fd) == -1)
		{
			safe_close(&cmd_ctx->prev_pipe_read);
			return (0);
		}
	}
	else
	{
		cmd_ctx->pipe_fd[0] = -1;
		cmd_ctx->pipe_fd[1] = -1;
	}
	return (1);
}

static pid_t	execute_command_process(t_cmd_ctx *cmd_ctx)
{
	pid_t	pid;
	char	**envp;

	envp = env_list_to_envp(*(cmd_ctx->env_list));
	if (!envp)
		return (-1);
	expand_command_args(cmd_ctx->current, envp);
	if (!setup_command_pipe(cmd_ctx))
		return (safe_doube_star_free(envp), -1);
	pid = fork();
	if (pid == -1)
		return (handle_fork_error(cmd_ctx), safe_doube_star_free(envp), -1);
	if (pid == 0)
		child_process(cmd_ctx);
	signal(SIGINT, SIG_IGN);
	safe_doube_star_free(envp);
	cmd_ctx->prev_pipe_read = parent_process(cmd_ctx->prev_pipe_read,
			cmd_ctx->pipe_fd);
	return (pid);
}

static pid_t	execute_command_pipeline(t_cmd_ctx *cmd_ctx,
		t_command *cmd_list)
{
	t_command	*last_cmd;
	pid_t		last_pid;

	last_pid = -1;
	last_cmd = cmd_list;
	while (last_cmd && last_cmd->next)
		last_cmd = last_cmd->next;
	while (cmd_ctx->current)
	{
		if (cmd_ctx->current == last_cmd)
			last_pid = execute_command_process(cmd_ctx);
		else
			execute_command_process(cmd_ctx);
		cmd_ctx->current = cmd_ctx->current->next;
	}
	return (last_pid);
}

int	execute_command_list(t_command *cmd_list, t_env **env_list)
{
	t_cmd_ctx	cmd_ctx;
	int			status;
	pid_t		last_pid;

	last_pid = -1;
	cmd_ctx.env_list = env_list;
	cmd_ctx.cmd_list = cmd_list;
	cmd_ctx.current = cmd_list;
	if (cmd_ctx.cmd_list && cmd_ctx.cmd_list->next == NULL
		&& cmd_ctx.cmd_list->args && cmd_ctx.cmd_list->args[0]
		&& is_parent_builtin(cmd_ctx.cmd_list->args[0]))
		return (execute_single_parent_builtin(&cmd_ctx));
	cmd_ctx.init_result = setup_pipes_and_heredocs(&cmd_ctx);
	if (cmd_ctx.init_result != 0)
		return (cmd_ctx.init_result);
	last_pid = execute_command_pipeline(&cmd_ctx, cmd_list);
	status = wait_for_specific_pid(last_pid);
	signal(SIGINT, sigint_handler);
	return ((cmd_ctx.status = status));
}
