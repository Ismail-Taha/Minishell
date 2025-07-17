/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:30:46 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:30:47 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	setup_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe creation failed");
		return (-1);
	}
	return (0);
}

int	handle_heredoc_redir(t_redirections *redir, char **envp)
{
	int	heredoc_fd;

	heredoc_fd = setup_heredoc(redir->file, envp);
	if (heredoc_fd == 130)
	{
		if (heredoc_fd > 0)
			safe_close(&heredoc_fd);
		return (130);
	}
	if (heredoc_fd == -1)
		return (-1);
	redir->heredoc_fd = heredoc_fd;
	return (0);
}

void	handle_fork_error(t_cmd_ctx *cmd_ctx)
{
	perror("minishell: fork error");
	safe_close(&cmd_ctx->prev_pipe_read);
	if (cmd_ctx->current->next)
	{
		safe_close(&cmd_ctx->pipe_fd[0]);
		safe_close(&cmd_ctx->pipe_fd[1]);
	}
}

void	handle_child_input(t_cmd_ctx *cmd_ctx)
{
	if (cmd_ctx->prev_pipe_read != -1)
	{
		if (dup2(cmd_ctx->prev_pipe_read, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 error on input");
			exit(1);
		}
		safe_close(&cmd_ctx->prev_pipe_read);
	}
}

void	handle_child_output(t_cmd_ctx *cmd_ctx)
{
	if (cmd_ctx->current->next)
	{
		safe_close(&cmd_ctx->pipe_fd[0]);
		if (dup2(cmd_ctx->pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 error on output");
			exit(1);
		}
		safe_close(&cmd_ctx->pipe_fd[1]);
	}
}
