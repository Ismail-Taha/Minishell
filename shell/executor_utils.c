#include "minishell.h"

int	setup_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe creation failed");
		return (-1);
	}
	return (0);
}

int	handle_heredoc_redir(t_redirections *redir)
{
	int	heredoc_fd;

	heredoc_fd = setup_heredoc(redir->file);
	if (heredoc_fd == -1)
	{
		fprintf(stderr, "minishell: heredoc setup failed for %s\n",
			redir->file);
		return (-1);
	}
	redir->heredoc_fd = heredoc_fd;
	return (0);
}

void	handle_fork_error(t_command *current, int prev_pipe_read,
		int pipe_fd[2])
{
	perror("minishell: fork error");
	safe_close(&prev_pipe_read);
	if (current->next)
	{
		safe_close(&pipe_fd[0]);
		safe_close(&pipe_fd[1]);
	}
}