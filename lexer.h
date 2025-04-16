/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ismail-Taha                                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:14:07 by Ismail-Taha       #+#    #+#             */
/*   Updated: 2025/04/16 04:14:07 by Ismail-Taha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>

/**
 * Enum for different token types in the shell
 */
typedef enum e_token_type
{
	TOKEN_WORD,           // Command or argument
	TOKEN_PIPE,           // '|' character
	TOKEN_REDIRECT_IN,    // '<' character
	TOKEN_REDIRECT_OUT,   // '>' character
	TOKEN_REDIRECT_APPEND,// '>>' characters
	TOKEN_HEREDOC,        // '<<' characters
	TOKEN_AND,            // '&&' characters
	TOKEN_OR,             // '||' characters
	TOKEN_SEMICOLON,      // ';' character
	TOKEN_EOF,            // End of input
	TOKEN_UNKNOWN         // Unknown token type
}	t_token_type;

/**
 * Structure to represent a token in the shell
 */
typedef struct s_token
{
	t_token_type	type;   // Type of the token
	char			*value; // String value of the token
	struct s_token	*next;  // Next token in the linked list
}	t_token;

/* Core lexer functions */
t_token	*create_token(t_token_type type, char *value);
void	add_token(t_token **tokens, t_token *new_token);
void	free_token_list(t_token *tokens);
t_token	*tokenize(char *input);
void	print_tokens(t_token *tokens);

/* Helper functions */
int		is_whitespace(char c);
int		is_special_char(char c);
int		handle_word(char *input, int *i, t_token **tokens);
int		handle_operator(char *input, int *i, t_token **tokens);

#endif