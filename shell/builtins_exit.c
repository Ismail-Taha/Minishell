#include "minishell.h"

static int	process_exit_arg(char *arg)
{
	int	exit_status;

	if (!is_number(arg))
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
			arg);
		return (2);
	}
	exit_status = ft_atoi(arg);
	return (exit_status);
}

int	builtin_exit(t_command *cmd)
{
	int	exit_status;

	printf("exit\n");
	if (cmd->args_count > 2)
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		return (1);
	}
	if (cmd->args_count > 1)
		exit_status = process_exit_arg(cmd->args[1]);
	else
		exit_status = 0;
	exit(exit_status);
}
