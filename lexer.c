/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 05:14:40 by marvin            #+#    #+#             */
/*   Updated: 2025/04/18 05:14:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Check if character is a whitespace
static bool is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

// Check if character is an operator
static bool is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

// Get the type of a character
static enum e_type get_char_type(char c)
{
	if (c == ' ')
		return SPACE;
	if (c == '\t')
		return TAB;
	if (c == '\n')
		return NEW_LINE;
	if (c == '|')
		return PIPE;
	if (c == '<')
		return REDIR_IN;
	if (c == '>')
		return REDIR_OUT;
	if (c == '\'')
		return SQUOTE;
	if (c == '"')
		return DQUOTE;
	if (c == '$')
		return ENV;
	return ARG; // Default is ARG type
}

// Check for multi-character operators (&&, ||, >>, <<)
static int check_multi_operator(char *line, int i, t_token **tokens)
{
	// Check for && (AND_IF)
	if (line[i] == '&' && line[i + 1] == '&')
	{
		add_token(tokens, new_token(strdup("&&"), AND_IF));
		return (i + 2);
	}
	// Check for || (OR_IF)
	else if (line[i] == '|' && line[i + 1] == '|')
	{
		add_token(tokens, new_token(strdup("||"), OR_IF));
		return (i + 2);
	}
	// Check for >> (APPEND)
	else if (line[i] == '>' && line[i + 1] == '>')
	{
		add_token(tokens, new_token(strdup(">>"), APPEND));
		return (i + 2);
	}
	// Check for << (HEREDOC)
	else if (line[i] == '<' && line[i + 1] == '<')
	{
		add_token(tokens, new_token(strdup("<<"), HEREDOC));
		return (i + 2);
	}
	return (i);
}

// Handle quoted content (single or double quotes)
static int handle_quotes(char *line, int i, t_token **tokens, enum e_type quote_type)
{
	int start = i + 1;
	int end = start;

	// Find the closing quote
	while (line[end] && line[end] != (quote_type == SQUOTE ? '\'' : '"'))
		end++;

	if (!line[end]) // Unclosed quote - will be caught by syntax checker
		return (i + 1);

	// Extract the quoted content
	char *content = malloc((end - start + 1) * sizeof(char));
	if (!content)
		return (i + 1);

	strncpy(content, line + start, end - start);
	content[end - start] = '\0';

	// Create token with appropriate type
	add_token(tokens, new_token(content, quote_type));

	return (end + 1);
}

// Extract a word (command or argument)
static int extract_word(char *line, int i, t_token **tokens)
{
	int start = i;

	// Find the end of the word
	while (line[i] && !is_whitespace(line[i]) && !is_operator(line[i])
		&& line[i] != '\'' && line[i] != '"')
		i++;

	// Extract the word
	char *content = malloc((i - start + 1) * sizeof(char));
	if (!content)
		return (i);

	strncpy(content, line + start, i - start);
	content[i - start] = '\0';

	// The first token is treated as a command, others as arguments
	enum e_type type = (*tokens == NULL) ? CMD : ARG;

	add_token(tokens, new_token(content, type));

	return (i);
}

// Create a new token with doubly linked list support
t_token	*new_token(char *content, enum e_type type)
{
	t_token *token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);

	token->content = content;
	token->type = type;
	token->next = NULL;
	token->prev = NULL;  // Initialize prev pointer

	return (token);
}

// Add a token to the list - with doubly linked list support
void	add_token(t_token **list, t_token *new_token)
{
	t_token *tmp;

	if (!new_token)
		return;

	if (!(*list))
	{
		*list = new_token;
		return;
	}

	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;

	tmp->next = new_token;
	new_token->prev = tmp;  // Set prev pointer to the current tail
}

// Free token list
void	free_tokens(t_token **tokens)
{
	t_token *tmp;

	if (!tokens || !(*tokens))
		return;

	while (*tokens)
	{
		tmp = (*tokens)->next;
		free((*tokens)->content);
		free(*tokens);
		*tokens = tmp;
	}
}

// Main tokenization function - OPTIMIZED with single pass
t_token	*tokenize(char *line)
{
	t_token *tokens = NULL;
	int i = 0;
	bool first_token = true; // Track if we're processing the first token

	// Process the entire line
	while (line[i])
	{
		// Skip whitespace but add as tokens
		if (is_whitespace(line[i]))
		{
			char whitespace[2] = {line[i], '\0'};
			add_token(&tokens, new_token(strdup(whitespace), get_char_type(line[i])));
			i++;
			continue;
		}

		// Check for multi-character operators
		int new_i = check_multi_operator(line, i, &tokens);
		if (new_i != i)
		{
			i = new_i;
			first_token = false;
			continue;
		}

		// Handle single-character operators
		if (is_operator(line[i]))
		{
			char op[2] = {line[i], '\0'};
			add_token(&tokens, new_token(strdup(op), get_char_type(line[i])));
			i++;
			first_token = false;
			continue;
		}

		// Handle quotes
		if (line[i] == '\'' || line[i] == '"')
		{
			i = handle_quotes(line, i, &tokens, line[i] == '\'' ? SQUOTE : DQUOTE);
			first_token = false;
			continue;
		}

		// Handle environment variables - marked for expansion later
		if (line[i] == '$')
		{
			// Just mark it as ENV, expansion happens after parsing
			char env[2] = {line[i], '\0'};
			add_token(&tokens, new_token(strdup(env), ENV));
			i++;
			first_token = false;
			continue;
		}

		// Handle regular words (commands or arguments)
		i = extract_word(line, i, &tokens);

		// Update first_token status
		if (first_token)
			first_token = false;
	}

	return (tokens);
}