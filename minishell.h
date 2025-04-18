/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:14:07 by Ismail-Taha       #+#    #+#             */
/*   Updated: 2025/04/18 05:21:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>        // printf, perror
# include <stdlib.h>
# include <unistd.h>       // fork, execve, access, pipe, dup2
# include <string.h>       // strcmp, strdup, strlen
# include <signal.h>       // signal handling
# include <fcntl.h>        // open, O_RDONLY, O_WRONLY, etc.
# include <ctype.h>        // isspace, isalpha, etc.
# include <sys/wait.h>     // wait, waitpid
# include <sys/stat.h>     // stat for file checking
# include <readline/readline.h> // command line input
# include <readline/history.h>  // command history
# include <errno.h>        // errno for error handling
# include <stdbool.h>      // bool type

// Forward declaration for environment variables
typedef struct s_env t_env;

// Token types
enum e_type
{
	CMD,      // Command
	ARG,      // Argument
	PIPE,     // Pipe operator (|)
	REDIR_IN, // Input redirection (<)
	REDIR_OUT, // Output redirection (>)
	APPEND,   // Append redirection (>>)
	HEREDOC,  // Here document (<<)
	SQUOTE,   // Single quote
	DQUOTE,   // Double quote
	SPACE,    // Space
	TAB,      // Tab
	NEW_LINE, // New line
	ENV,      // Environment variable
	AND_IF,   // Logical AND (&&)
	OR_IF,    // Logical OR (||)
};

// Token structure - now with doubly linked list
typedef struct s_token
{
	char			*content;  // Token content
	enum e_type		type;      // Token type
	struct s_token	*next;     // Next token
	struct s_token	*prev;     // Previous token
}	t_token;

// Redirection structure - represents input/output redirections
typedef struct s_redir
{
	char			*file;     // Filename or delimiter
	enum e_type		type;      // Type of redirection
	int             fd;        // File descriptor (optimization: store FD here)
	struct s_redir	*next;     // Next redirection
}	t_redir;

// Command structure - represents a simple command
typedef struct s_cmd
{
	char	**cmd;            // Command and arguments
	t_redir	*redir;           // Redirections list
	bool    is_builtin;       // Optimization: flag if command is builtin
}	t_cmd;

// Tree node - the main structure for the AST
typedef struct s_tree
{
	enum e_type		type;      // Type of the node
	void			*content;  // Content based on node type
	struct s_tree	*left;     // Left child
	struct s_tree	*right;    // Right child
}	t_tree;

// Token functions
t_token	*tokenize(char *line);
t_token	*new_token(char *content, enum e_type type);
void	add_token(t_token **list, t_token *new_token);
void	free_tokens(t_token **tokens);

// Syntax checking
bool	check_syntax(t_token *tokens);

// Tree functions
t_tree	*parse(t_token *tokens);
t_tree	*new_tree_node(enum e_type type, void *content);
void	add_tree_node(t_tree **root, t_tree *node, char side); // 'L' for left, 'R' for right
void	free_tree(t_tree **tree);

// Redirection functions
t_redir	*new_redir_node(char *file, enum e_type type);
void	add_redir_node(t_redir **lst, t_redir *new);
void	free_redirs(t_redir **redirs);

// Environment variable handling
void	expand_env_vars(t_tree *tree, t_env *env);

// Utility functions
bool	is_operator_token(enum e_type type);
bool	is_redir_token(enum e_type type);
char	**dup_cmd_array(char **cmd);

#endif