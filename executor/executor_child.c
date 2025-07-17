/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:30:38 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:30:39 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_process(t_cmd_ctx *cmd_ctx)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	handle_child_input(cmd_ctx);
	handle_child_output(cmd_ctx);
	if (setup_redirections(cmd_ctx->current) == -1)
		exit(1);
	execute_single_command(cmd_ctx);
	ft_fprintf_fd(2, "minishell: command execution failed\n");
	exit(127);
}

static int	handle_process_status(pid_t pid, int status, pid_t last_pid)
{
	int	last_command_status;
	int	exit_status;

	last_command_status = 0;
	exit_status = 0;
	if (pid == last_pid)
	{
		if (WIFEXITED(status))
			last_command_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_command_status = 128 + WTERMSIG(status);
	}
	if (WIFSIGNALED(status))
	{
		exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT && g_last_exit_status != 131)
			write(STDERR_FILENO, "Quit: 3\n", 9);
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
	}
	else if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	if (pid == last_pid)
		return (last_command_status);
	return (exit_status);
}

int	wait_for_specific_pid(pid_t last_pid)
{
	pid_t	pid;
	int		status;
	int		last_status;
	int		last_command_status;

	status = 0;
	last_status = 0;
	last_command_status = 0;
	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (pid == last_pid)
			last_command_status = handle_process_status(pid, status, last_pid);
		else
			last_status = handle_process_status(pid, status, -1);
		pid = waitpid(-1, &status, 0);
	}
	if (last_pid != -1)
		return (last_command_status);
	return (last_status);
}

int	parent_process(int prev_pipe_read, int pipe_fd[2])
{
	if (prev_pipe_read != -1)
		safe_close(&prev_pipe_read);
	if (pipe_fd[1] != -1)
	{
		safe_close(&pipe_fd[1]);
		return (pipe_fd[0]);
	}
	if (pipe_fd[0] != -1)
		return (pipe_fd[0]);
	return (-1);
}

int	setup_all_heredocs(t_command *cmd_list, char **envp)
{
	t_command		*current;
	t_redirections	*redir;
	int				result;

	current = cmd_list;
	while (current)
	{
		redir = current->redirections;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
			{
				result = handle_heredoc_redir(redir, envp);
				if (result == 130)
					return (130);
				if (result == -1)
					return (-1);
			}
			redir = redir->next;
		}
		current = current->next;
	}
	return (0);
}
