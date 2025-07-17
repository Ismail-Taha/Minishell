/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:30:49 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 18:08:25 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_heredoc_line(char *line, int pipe_fd, int quoted,
		char **envp)
{
	char	*expanded;

	if (!quoted)
	{
		expanded = expand_variables(line, envp);
		if (expanded)
		{
			ft_fprintf_fd(pipe_fd, "%s", expanded);
			safe_free((void **)&expanded);
		}
		else
			ft_fprintf_fd(pipe_fd, "%s", line);
	}
	else
		ft_fprintf_fd(pipe_fd, "%s", line);
	ft_putchar_fd('\n', pipe_fd);
}

static void	read_heredoc_lines(int pipe_fd, char *processed_delimiter,
		int quoted, char **envp)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_fprintf_fd(2, "minishell: warning: ");
			ft_fprintf_fd(2, "heredoc delimited by %s end-of-file\n",
				processed_delimiter);
			g_last_exit_status = 0;
			break ;
		}
		if (ft_strcmp(line, processed_delimiter) == 0)
		{
			safe_free((void **)&line);
			break ;
		}
		process_heredoc_line(line, pipe_fd, quoted, envp);
		safe_free((void **)&line);
	}
}

static void	handle_heredoc_child_process(int pipe_fd[2], char *delimiter,
		char **envp)
{
	int		quoted;
	char	*processed_delimiter;

	signal(SIGINT, child_sigint_handler);
	safe_close(&pipe_fd[0]);
	quoted = is_content_quoted(delimiter);
	processed_delimiter = strip_quotes(delimiter);
	if (!processed_delimiter)
		exit(1);
	read_heredoc_lines(pipe_fd[1], processed_delimiter, quoted, envp);
	safe_free((void **)&processed_delimiter);
	safe_close(&pipe_fd[1]);
	exit(0);
}

static int	handle_heredoc_parent(int pipe_fd[2], pid_t pid)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	safe_close(&pipe_fd[1]);
	waitpid(pid, &status, 0);
	if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT))
	{
		safe_close(&pipe_fd[0]);
		g_last_exit_status = 130;
		return (130);
	}
	signal(SIGINT, sigint_handler);
	return (pipe_fd[0]);
}

int	setup_heredoc(char *delimiter, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid;

	signal(SIGINT, heredoc_sigint_handler);
	if (pipe(pipe_fd) == -1)
		return (ft_fprintf_fd(2, "minishell: heredoc pipe error\n"), -1);
	pid = fork();
	if (pid == -1)
	{
		safe_close(&pipe_fd[0]);
		safe_close(&pipe_fd[1]);
		return (ft_fprintf_fd(2, "minishell: heredoc fork error\n"), -1);
	}
	if (pid == 0)
		handle_heredoc_child_process(pipe_fd, delimiter, envp);
	return (handle_heredoc_parent(pipe_fd, pid));
}
