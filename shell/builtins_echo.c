#include "minishell.h"

static int	is_valid_n_flag(char *arg)
{
	int	i;

	if (arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_echo(t_command *cmd)
{
	int	i;
	int	print_newline;

	i = 1;
	print_newline = 1;
	while (i < cmd->args_count && is_valid_n_flag(cmd->args[i]))
	{
		print_newline = 0;
		i++;
	}
	while (i < cmd->args_count)
	{
		printf("%s", cmd->args[i]);
		if (i < cmd->args_count - 1)
			printf(" ");
		i++;
	}
	if (print_newline)
		printf("\n");
	return (0);
}
