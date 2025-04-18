#include "minishell.h"

void	safe_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	safe_close(int *fd)
{
	if (fd && *fd >= 0)
	{
		close(*fd);
		*fd = -1;
	}
}

void	free_command(t_command *cmd)
{
	t_redirections	*redir;
	t_redirections	*next_redir;
	int				i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (i < cmd->args_count)
		{
			safe_free((void **)&cmd->args[i]);
			i++;
		}
		safe_free((void **)&cmd->args);
	}
	redir = cmd->redirections;
	while (redir)
	{
		next_redir = redir->next;
		if (redir->type == TOKEN_HEREDOC && redir->heredoc_fd >= 0)
			safe_close(&redir->heredoc_fd);
		safe_free((void **)&redir->file);
		safe_free((void **)&redir);
		redir = next_redir;
	}
	safe_free((void **)&cmd);
}

void	free_command_list(t_command *cmd_list)
{
	t_command	*current;
	t_command	*next;

	current = cmd_list;
	while (current)
	{
		next = current->next;
		free_command(current);
		current = next;
	}
}
