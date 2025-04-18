#include "minishell.h"

int	g_last_exit_status = 0;

static void	display_welcome(void)
{
	printf("\n");
	printf(" ╔════════════════════════════════════╗\n");
	printf(" ║            MINISHELL               ║\n");
	printf(" ╚════════════════════════════════════╝\n");
	printf(" Welcome to Minishell! Type 'exit' to quit.\n\n");
}

void	print_Cmd_list(t_command *cmd)
{
	int	i = 0;
	while (cmd)
	{
		i = 0;
		while (i < cmd->args_count)
			printf("%s ", cmd->args[i++]);
		printf("\n");
		cmd = cmd->next;
	}
}

static int	process_command(char *input, t_env **env_list)
{
	t_token		*tokens;
	t_command	*cmds;

	tokens = NULL;
	cmds = NULL;
	tokens = tokenize_input(input);
	if (!tokens)
		return (0);
	assign_token_types(tokens);
	cmds = create_cmds(&tokens);
	if (cmds)
	{
		g_last_exit_status = execute_command_list(cmds, env_list);
		printf("Command exit status: %d\n", g_last_exit_status);
		free_command_list(cmds);
	}
	else if (tokens)
	{
		fprintf(stderr, "minishell: syntax error in command\n");
		ft_token_clear(&tokens, free);
	}
	return (0);
}

static void	shell_loop(t_env *env_list)
{
	char	*input;
	int		should_exit;

	should_exit = 0;
	while (!should_exit)
	{
		input = readline("minishell> ");
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (input[0] != '\0')
		{
			add_history(input);
			should_exit = process_command(input, &env_list);
		}
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_list;

	(void)argc;
	(void)argv;
	setup_signals();
	display_welcome();
	env_list = envp_to_env_list(envp);
	shell_loop(env_list);
	// TODO: free env_list at the end
	return (0);
}