/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:49:14 by Ismail-Taha       #+#    #+#             */
/*   Updated: 2025/04/16 15:54:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

/* Token types */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_SEMICOLON,
	TOKEN_EOF,
	TOKEN_UNKNOWN
} t_token_type;

/* Token structure */
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
} t_token;

/* Lexer function prototypes */
t_token	*create_token(t_token_type type, char *value);
void	add_token(t_token **tokens, t_token *new_token);
void	free_token_list(t_token *tokens);
int		is_whitespace(char c);
int		is_special_char(char c);
int		process_quoted_string(char *input, int *i, char quote_type, char *result);
int		handle_word(char *input, int *i, t_token **tokens);
int		handle_operator(char *input, int *i, t_token **tokens);
int		is_valid_var_char(char c);
int		identify_variable(char *input, int *i);
t_token	*tokenize(char *input);
void	print_tokens(t_token *tokens);

#endif