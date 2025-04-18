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

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <fcntl.h>
# include <ctype.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h> 
# include <errno.h> 
# include <stdbool.h>
typedef struct s_env t_env;

enum e_type
{
	CMD,      // cmd
	ARG,      // argument
	PIPE,     // (|)
	REDIR_IN, // (<)
	REDIR_OUT, // (>)
	APPEND,   // (>>)
	HEREDOC,  // (<<)
	SQUOTE,
	DQUOTE,
	SPACE, // (32)
	TAB, // (\t)
	NEW_LINE,
	ENV,
	AND_IF,   // (&&)
	OR_IF,    // (||)
};

// token structure
typedef struct s_token
{
	char			*content;
	enum e_type		type;    
	struct s_token	*next;   
	struct s_token	*prev;
}	t_token;

// input/output redirections
typedef struct s_redir
{
	char			*file;     // Filename or delimiter
	enum e_type		type;      // Type of redirection
	int             fd;        // File descriptor (optimization: store FD here)
	struct s_redir	*next;     // Next redirection
}	t_redir;

// command structure
typedef struct s_cmd
{
	char	**cmd;
	t_redir	*redir;           // redirections list
	bool    is_builtin;       // flag if command is builtin
}	t_cmd;

// tree structure
typedef struct s_tree
{
	enum e_type		type;
	void			*content;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

// lexer functions
t_token	*tokenize(char *line);
t_token	*new_token(char *content, enum e_type type);
void	add_token(t_token **list, t_token *new_token);
void	free_tokens(t_token **tokens);

bool	check_syntax(t_token *tokens);

// tree functions utils
t_tree	*parse(t_token *tokens);
t_tree	*new_tree_node(enum e_type type, void *content);
void	add_tree_node(t_tree **root, t_tree *node, char side);
void	free_tree(t_tree **tree);

// redirection functions
t_redir	*new_redir_node(char *file, enum e_type type);
void	add_redir_node(t_redir **lst, t_redir *new);
void	free_redirs(t_redir **redirs);

void	expand_env_vars(t_tree *tree, t_env *env);

bool	is_operator_token(enum e_type type);
bool	is_redir_token(enum e_type type);
char	**dup_cmd_array(char **cmd);

#endif