#include "minishell.h"

static t_command	*handle_word_token(t_token **current, t_command **first_cmd,
		t_command **current_cmd)
{
	t_command	*new_cmd;

	new_cmd = create_command_type_word(current);
	if (!new_cmd)
		return (NULL);
	if (!*first_cmd)
		*first_cmd = new_cmd;
	else if (*current_cmd)
		(*current_cmd)->next = new_cmd;
	*current_cmd = new_cmd;
	return (new_cmd);
}

static int	handle_pipe_token(t_command *current_cmd)
{
	if (!current_cmd)
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `|\n");
		return (0);
	}
	return (1);
}

t_command	*create_cmds(t_token **tokens)
{
	t_command	*first_cmd;
	t_command	*current_cmd;
	t_token		*current;

	first_cmd = NULL;
	current_cmd = NULL;
	current = *tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			if (!handle_word_token(&current, &first_cmd, &current_cmd))
				return (NULL);
		}
		else if (current->type == TOKEN_PIPE)
		{
			if (!handle_pipe_token(current_cmd))
				return (NULL);
			current = current->next;
		}
		else if (current->type == TOKEN_REDIRECT_IN
			|| current->type == TOKEN_REDIRECT_OUT
			|| current->type == TOKEN_APPEND || current->type == TOKEN_HEREDOC)
		{
			if (!handle_redirect_token(&current, &first_cmd, &current_cmd))
				return (NULL);
		}
		else
			current = current->next;
	}
	return (first_cmd);
}
