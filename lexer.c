/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ismail-Taha                                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:14:07 by Ismail-Taha       #+#    #+#             */
/*   Updated: 2025/04/16 04:14:07 by Ismail-Taha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Creates a new token with the specified type and value
 * 
 * @param type The token type
 * @param value The string value of the token
 * @return Pointer to the newly created token, NULL if malloc fails
 */
t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->next = NULL;
	return (token);
}

/**
 * Adds a new token to the end of the token list
 * 
 * @param tokens Pointer to the head of the token list
 * @param new_token Token to be added
 */
void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!new_token)
		return ;
	if (*tokens == NULL)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

/**
 * Frees all tokens in the list
 * 
 * @param tokens Head of the token list
 */
void	free_token_list(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}

/**
 * Checks if a character is a whitespace
 * 
 * @param c Character to check
 * @return 1 if whitespace, 0 otherwise
 */
int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

/**
 * Checks if a character is a special operator character
 * 
 * @param c Character to check
 * @return 1 if special character, 0 otherwise
 */
int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&' ||
			c == ';' || c == '(' || c == ')');
}

/**
 * Processes a word token (command or argument)
 * 
 * @param input Input string
 * @param i Current position in the input
 * @param tokens List to add the new token to
 * @return 0 on success, -1 on error
 */
int	handle_word(char *input, int *i, t_token **tokens)
{
	int		start;
	int		in_quotes;
	char	quote_char;
	char	*word;

	start = *i;
	in_quotes = 0;
	quote_char = 0;
	while (input[*i] && (in_quotes || (!is_whitespace(input[*i]) && !is_special_char(input[*i]))))
	{
		// Handle quotes
		if ((input[*i] == '"' || input[*i] == '\'') && (!in_quotes || quote_char == input[*i]))
		{
			if (in_quotes)
				in_quotes = 0;
			else
			{
				in_quotes = 1;
				quote_char = input[*i];
			}
		}
		// Handle escape sequences (if needed for your shell)
		else if (input[*i] == '\\' && input[*i + 1])
			(*i)++;
		(*i)++;
	}
	if (in_quotes) // Unclosed quote
		return (-1);
	word = ft_substr(input, start, *i - start);
	if (!word)
		return (-1);
	add_token(tokens, create_token(TOKEN_WORD, word));
	free(word);
	return (0);
}

/**
 * Processes operator tokens (|, <, >, etc)
 * 
 * @param input Input string
 * @param i Current position in the input
 * @param tokens List to add the new token to
 * @return 0 on success
 */
int	handle_operator(char *input, int *i, t_token **tokens)
{
	char		op[3];
	t_token_type	type;

	op[0] = input[*i];
	op[1] = '\0';
	op[2] = '\0';
	(*i)++;

	// Handle two-character operators
	if ((op[0] == '>' && input[*i] == '>') ||
		(op[0] == '<' && input[*i] == '<') ||
		(op[0] == '&' && input[*i] == '&') ||
		(op[0] == '|' && input[*i] == '|'))
	{
		op[1] = input[*i];
		(*i)++;
	}

	// Set token type based on operator
	if (op[0] == '|' && op[1] == '\0')
		type = TOKEN_PIPE;
	else if (op[0] == '|' && op[1] == '|')
		type = TOKEN_OR;
	else if (op[0] == '&' && op[1] == '&')
		type = TOKEN_AND;
	else if (op[0] == '<' && op[1] == '\0')
		type = TOKEN_REDIRECT_IN;
	else if (op[0] == '>' && op[1] == '\0')
		type = TOKEN_REDIRECT_OUT;
	else if (op[0] == '<' && op[1] == '<')
		type = TOKEN_HEREDOC;
	else if (op[0] == '>' && op[1] == '>')
		type = TOKEN_REDIRECT_APPEND;
	else if (op[0] == ';')
		type = TOKEN_SEMICOLON;
	else
		type = TOKEN_UNKNOWN;

	add_token(tokens, create_token(type, op));
	return (0);
}

/**
 * Main tokenization function - converts input string to a list of tokens
 * 
 * @param input Command line input string
 * @return Linked list of tokens, NULL if error
 */
t_token	*tokenize(char *input)
{
	t_token	*tokens;
	int		i;
	int		error;

	if (!input)
		return (NULL);
	tokens = NULL;
	i = 0;
	error = 0;
	while (input[i] && !error)
	{
		// Skip whitespace
		if (is_whitespace(input[i]))
		{
			i++;
			continue;
		}
		// Handle operators
		if (is_special_char(input[i]))
			error = handle_operator(input, &i, &tokens);
		// Handle normal words
		else
			error = handle_word(input, &i, &tokens);
	}
	// Add EOF token to signal end of input
	if (!error)
		add_token(&tokens, create_token(TOKEN_EOF, ""));
	else
	{
		free_token_list(tokens);
		tokens = NULL;
	}
	return (tokens);
}

/**
 * Debugging function to print all tokens in the list
 * 
 * @param tokens Head of token list
 */
void	print_tokens(t_token *tokens)
{
	t_token	*current;
	char	*type_str;

	current = tokens;
	printf("TOKEN LIST:\n");
	while (current)
	{
		// Convert token type to string for display
		switch (current->type)
		{
			case TOKEN_WORD: type_str = "WORD"; break;
			case TOKEN_PIPE: type_str = "PIPE"; break;
			case TOKEN_REDIRECT_IN: type_str = "REDIR_IN"; break;
			case TOKEN_REDIRECT_OUT: type_str = "REDIR_OUT"; break;
			case TOKEN_REDIRECT_APPEND: type_str = "REDIR_APPEND"; break;
			case TOKEN_HEREDOC: type_str = "HEREDOC"; break;
			case TOKEN_AND: type_str = "AND"; break;
			case TOKEN_OR: type_str = "OR"; break;
			case TOKEN_SEMICOLON: type_str = "SEMICOLON"; break;
			case TOKEN_EOF: type_str = "EOF"; break;
			default: type_str = "UNKNOWN"; break;
		}
		printf("[%s] '%s'\n", type_str, current->value);
		current = current->next;
	}
}