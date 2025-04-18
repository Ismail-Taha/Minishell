/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 05:14:48 by marvin            #+#    #+#             */
/*   Updated: 2025/04/18 05:14:48 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Check for balanced quotes
static bool check_balanced_quotes(t_token *tokens)
{
	bool in_squote = false;
	bool in_dquote = false;

	t_token *token = tokens;
	while (token)
	{
		if (token->type == SQUOTE)
			in_squote = !in_squote;
		else if (token->type == DQUOTE)
			in_dquote = !in_dquote;

		token = token->next;
	}

	return (!in_squote && !in_dquote);
}

// Check for invalid pipe usage
static bool check_pipes(t_token *tokens)
{
	bool seen_cmd = false;
	t_token *token = tokens;

	while (token)
	{
		// Skip whitespace
		if (token->type == SPACE || token->type == TAB || token->type == NEW_LINE)
		{
			token = token->next;
			continue;
		}

		// Check for pipe at the beginning or consecutive pipes
		if (token->type == PIPE || token->type == AND_IF || token->type == OR_IF)
		{
			if (!seen_cmd)
				return (false);

			// Reset cmd flag after operator
			seen_cmd = false;
		}
		else if (token->type == CMD || token->type == ARG)
		{
			seen_cmd = true;
		}

		token = token->next;
	}

	// Check for trailing pipe
	return (seen_cmd);
}

// Check for invalid redirection usage
static bool check_redirections(t_token *tokens)
{
	t_token *token = tokens;

	while (token)
	{
		if (token->type == REDIR_IN || token->type == REDIR_OUT ||
			token->type == APPEND || token->type == HEREDOC)
		{
			// Skip whitespace
			token = token->next;
			while (token && (token->type == SPACE || token->type == TAB))
				token = token->next;

			// Missing filename after redirection
			if (!token || token->type == PIPE || token->type == AND_IF ||
				token->type == OR_IF || token->type == REDIR_IN ||
				token->type == REDIR_OUT || token->type == APPEND ||
				token->type == HEREDOC)
				return (false);
		}

		if (token)
			token = token->next;
	}

	return (true);
}

// Main syntax check function
bool check_syntax(t_token *tokens)
{
	// Empty input is valid
	if (!tokens)
		return (true);

	return (check_balanced_quotes(tokens) &&
			check_pipes(tokens) &&
			check_redirections(tokens));
}