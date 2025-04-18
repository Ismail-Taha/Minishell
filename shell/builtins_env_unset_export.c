#include "minishell.h"

int	builtin_export(t_command *cmd, t_env **env_list)
{
	int	i;

	if (!cmd->args[1])
	{
		print_export_list(*env_list);
		return (0);
	}
	i = 1;
	while (cmd->args[i])
	{
		export_one_arg(cmd->args[i], env_list);
		i++;
	}
	return (0);
}

int	builtin_unset(t_command *cmd, t_env **env_list)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		unset_one_arg(cmd->args[i], env_list);
		i++;
	}
	return (0);
}

int	builtin_env(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}
