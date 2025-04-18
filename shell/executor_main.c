#include "minishell.h"

int	is_builtin_command(char *cmd)
{
	char	*builtins[] = {"cd", "echo", "pwd", "export", "unset", "env",
			"exit", NULL};
	int		i;

	i = -1;
	while (builtins[++i])
	{
		if (ft_strcmp(builtins[i], cmd) == 0)
			return (1);
	}
	return (0);
}

int	execute_builtin(t_command *cmd, t_env **env_list)
{
	char	*command;

	command = cmd->args[0];
	if (ft_strcmp(command, "cd") == 0)
		return (builtin_cd(cmd));
	else if (ft_strcmp(command, "echo") == 0)
		return (builtin_echo(cmd));
	else if (ft_strcmp(command, "pwd") == 0)
		return (builtin_pwd());
	else if (ft_strcmp(command, "export") == 0)
		return (builtin_export(cmd, env_list));
	else if (ft_strcmp(command, "unset") == 0)
		return (builtin_unset(cmd, env_list));
	else if (ft_strcmp(command, "env") == 0)
		return (builtin_env(*env_list));
	else if (ft_strcmp(command, "exit") == 0)
		return (builtin_exit(cmd));
	return (1);
}

int	setup_all_heredocs(t_command *cmd_list)
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
				result = handle_heredoc_redir(redir);
				if (result == -1)
					return (-1);
			}
			redir = redir->next;
		}
		current = current->next;
	}
	return (0);
}

static void	close_heredoc_fds(t_redirections *redir)
{
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC && redir->heredoc_fd >= 0)
			safe_close(&redir->heredoc_fd);
		redir = redir->next;
	}
}

int	execute_command_list(t_command *cmd_list, t_env **env_list)
{
	int			pipe_fd[2];
	pid_t		pid;
	int			prev_pipe_read;
	int			status;
	t_command	*current;

	if (cmd_list && cmd_list->next == NULL
		&& is_parent_builtin(cmd_list->args[0]))
	{
		expand_command_args(cmd_list, env_list_to_envp(*env_list));
		status = execute_builtin(cmd_list, env_list);
		return (status);
	}
	if (setup_all_heredocs(cmd_list) == -1)
		return (1);
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	prev_pipe_read = -1;
	current = cmd_list;
	while (current)
	{
		if (!setup_command_pipe(current, &prev_pipe_read, pipe_fd))
			return (1);
		expand_command_args(current, env_list_to_envp(*env_list));
		pid = fork();
		if (pid == -1)
		{
			handle_fork_error(current, prev_pipe_read, pipe_fd);
			return (1);
		}
		if (pid == 0)
			child_process(current, prev_pipe_read, pipe_fd, *env_list);
		close_heredoc_fds(current->redirections);
		prev_pipe_read = parent_process(prev_pipe_read, pipe_fd);
		current = current->next;
	}
	safe_close(&prev_pipe_read);
	return (wait_for_children());
}
