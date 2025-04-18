#include "minishell.h"

int	setup_heredoc(char *delimiter)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: heredoc pipe error");
		return (-1);
	}
	printf("heredoc ");
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			fprintf(stderr,
				"minishell: warning: heredoc delimited by end-of-file\n");
			safe_close(&pipe_fd[1]);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			safe_close(&pipe_fd[1]);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	safe_close(&pipe_fd[1]);
	return (pipe_fd[0]);
}
