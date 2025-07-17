/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:50:22 by isallali          #+#    #+#             */
/*   Updated: 2025/05/07 17:50:23 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		ft_fprintf_fd(2, "minishell: syntax error near unexpected token `|'\n");
		g_last_exit_status = 2;
		return (0);
	}
	return (1);
}

static int	handle_pipe_with_redirection(t_token *current,
		t_command **current_cmd)
{
	t_command	*new_cmd;

	if (current->next && ((current)->type == TOKEN_REDIRECT_IN
			|| (current)->type == TOKEN_HEREDOC
			|| (current)->type == TOKEN_REDIRECT_OUT
			|| (current)->type == TOKEN_APPEND))
	{
		new_cmd = command_init();
		if (!new_cmd)
			return (0);
		(*current_cmd)->next = new_cmd;
		*current_cmd = new_cmd;
	}
	return (1);
}

static int	process_token(t_token **current, t_command **first_cmd,
		t_command **current_cmd)
{
	if ((*current)->type == TOKEN_WORD)
	{
		if (!handle_word_token(current, first_cmd, current_cmd))
			return (0);
	}
	else if ((*current)->type == TOKEN_PIPE)
	{
		if (!handle_pipe_token(*current_cmd)
			|| !handle_pipe_with_redirection(*current, current_cmd))
			return (0);
		*current = (*current)->next;
	}
	else if ((*current)->type == TOKEN_REDIRECT_IN
		|| (*current)->type == TOKEN_HEREDOC
		|| (*current)->type == TOKEN_REDIRECT_OUT
		|| (*current)->type == TOKEN_APPEND)
	{
		if (!handle_redirect_token(current, first_cmd, current_cmd))
			return (0);
	}
	else
		*current = (*current)->next;
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
		if (!process_token(&current, &first_cmd, &current_cmd))
		{
			if (first_cmd)
				free_command_list(first_cmd);
			return (NULL);
		}
	}
	return (finish_command_parsing(first_cmd));
}
