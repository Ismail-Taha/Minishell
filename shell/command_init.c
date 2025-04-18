#include "minishell.h"

t_command	*command_init(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->args_count = 0;
	cmd->redirections = NULL;
	cmd->next = NULL;
	return (cmd);
}

static int	count_word_args(t_token *token)
{
	int	count;

	count = 0;
	while (token && token->type == TOKEN_WORD)
	{
		count++;
		token = token->next;
	}
	return (count);
}

static void	fill_word_args(char **args, t_token **token, int count)
{
	int	i;

	i = 0;
	while (*token && (*token)->type == TOKEN_WORD && i < count)
	{
		args[i] = ft_strdup((*token)->content);
		*token = (*token)->next;
		i++;
	}
	args[i] = NULL;
}

t_command	*create_command_type_word(t_token **tokens)
{
	t_command *cmds;
	int args_count;

	if (!*tokens || (*tokens)->type != TOKEN_WORD)
		return (NULL);
	cmds = command_init();
	if (!cmds)
		return (NULL);
	args_count = count_word_args(*tokens);
	cmds->args = malloc(sizeof(char *) * (args_count + 1));
	if (!cmds->args)
		return (free(cmds), NULL);
	fill_word_args(cmds->args, tokens, args_count);
	cmds->args_count = args_count;
	return (cmds);
}