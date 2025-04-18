#include "minishell.h"

static void	expand_args_loop(t_command *cmd, char **envp)
{
	int		i;
	char	*expanded;
	char	*stripped;

	i = 0;
	while (i < cmd->args_count)
	{
		expanded = expand_variables(cmd->args[i], envp);
		if (expanded)
		{
			free(cmd->args[i]);
			cmd->args[i] = expanded;
		}
		stripped = strip_quotes(cmd->args[i]);
		if (stripped)
		{
			free(cmd->args[i]);
			cmd->args[i] = stripped;
		}
		i++;
	}
}

static void	expand_redirections_loop(t_command *cmd, char **envp)
{
	t_redirections	*redir;
	char			*expanded;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type != TOKEN_HEREDOC)
		{
			expanded = expand_variables(redir->file, envp);
			if (expanded)
			{
				free(redir->file);
				redir->file = expanded;
			}
		}
		redir = redir->next;
	}
}

void	expand_command_args(t_command *cmd, char **envp)
{
	expand_args_loop(cmd, envp);
	expand_redirections_loop(cmd, envp);
}
