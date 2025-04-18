#include "minishell.h"

static char	*get_exec_path(t_command *current, t_env *env_list)
{
	char	**envp;
	char	*exec_path;
	int		i;

	envp = env_list_to_envp(env_list);
	exec_path = find_executable_path(current->args[0], envp);
	i = 0;
	while (envp && envp[i])
		free(envp[i++]);
	free(envp);
	if (!exec_path)
		exec_path = ft_strdup(current->args[0]);
	return (exec_path);
}

void	handle_builtin_command(t_command *current, t_env *env_list)
{
	exit(execute_builtin(current, &env_list));
}

void	handle_external_command(t_command *current, t_env *env_list)
{
	char	*exec_path;
	char	**envp;
	int		i;

	exec_path = get_exec_path(current, env_list);
	if (!exec_path)
	{
		fprintf(stderr, "minishell: %s: command not found\n", current->args[0]);
		exit(127);
	}
	envp = env_list_to_envp(env_list);
	execve(exec_path, current->args, envp);
	perror("execve error");
	free(exec_path);
	i = 0;
	while (envp && envp[i])
		free(envp[i++]);
	free(envp);
	exit(127);
}

void	execute_single_command(t_command *current, t_env *env_list)
{
	if (current->args && current->args[0]
		&& is_builtin_command(current->args[0]))
		handle_builtin_command(current, env_list);
	else if (current->args && current->args[0])
		handle_external_command(current, env_list);
	exit(0);
}

int	setup_command_pipe(t_command *current, int *prev_pipe_read, int pipe_fd[2])
{
	if (current->next)
	{
		if (setup_pipe(pipe_fd) == -1)
		{
			safe_close(prev_pipe_read);
			return (0);
		}
	}
	else
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
	}
	return (1);
}

int	is_parent_builtin(char *cmd)
{
	return (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "exit") == 0);
}
