#include "minishell.h"

void	add_redirection(t_command *cmd, t_token_type redirect_type, char *file)
{
	t_redirections	*redirection;
	t_redirections	*current;

	redirection = malloc(sizeof(t_redirections));
	if (!redirection)
		return ;
	redirection->type = redirect_type;
	redirection->file = ft_strdup(file);
	redirection->heredoc_fd = -1;
	redirection->next = NULL;
	if (!redirection->file)
		return (free(redirection));
	if (!cmd->redirections)
		cmd->redirections = redirection;
	else
	{
		current = cmd->redirections;
		while (current->next)
			current = current->next;
		current->next = redirection;
	}
}

int	setup_redirections(t_command *cmd)
{
	t_redirections	*redir;
	int				result;

	redir = cmd->redirections;
	result = 0;
	while (redir && result == 0)
	{
		if (redir->type == TOKEN_REDIRECT_IN)
			result = setup_redirect_in(redir->file);
		else if (redir->type == TOKEN_REDIRECT_OUT)
			result = setup_redirect_out(redir->file, 0);
		else if (redir->type == TOKEN_APPEND)
			result = setup_redirect_out(redir->file, 1);
		else if (redir->type == TOKEN_HEREDOC)
		{
			if (dup2(redir->heredoc_fd, STDIN_FILENO) == -1)
			{
				fprintf(stderr, "minishell: heredoc redirection failed: %s\n",
					strerror(errno));
				result = -1;
			}
			safe_close(&redir->heredoc_fd);
		}
		else
		{
			fprintf(stderr, "minishell: unknown redirection type\n");
			result = -1;
		}
		redir = redir->next;
	}
	return (result);
}

int	setup_redirect_in(char *file_path)
{
	int	fd;

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "minishell: %s: %s\n", file_path, strerror(errno));
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		fprintf(stderr, "minishell: input redirection failed: %s\n",
			strerror(errno));
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	setup_redirect_out(char *file_path, int append_mode)
{
	int	flags;
	int	fd;

	flags = O_WRONLY | O_CREAT;
	flags |= (append_mode ? O_APPEND : O_TRUNC);
	fd = open(file_path, flags, 0644);
	if (fd == -1)
	{
		fprintf(stderr, "minishell: %s: %s\n", file_path, strerror(errno));
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		fprintf(stderr, "minishell: output redirection failed: %s\n",
			strerror(errno));
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_redirect_token(t_token **current, t_command **first_cmd,
		t_command **current_cmd)
{
	t_command		*new_cmd;
	t_token_type	redirect_type;

	if (!*current_cmd)
	{
		new_cmd = command_init();
		if (!new_cmd)
			return (0);
		if (!*first_cmd)
			*first_cmd = new_cmd;
		*current_cmd = new_cmd;
	}
	redirect_type = (*current)->type;
	*current = (*current)->next;
	if (!*current || (*current)->type != TOKEN_WORD)
	{
		fprintf(stderr, "minishell: syntax error near unexpected token\n");
		return (0);
	}
	add_redirection(*current_cmd, redirect_type, (*current)->content);
	*current = (*current)->next;
	return (1);
}
