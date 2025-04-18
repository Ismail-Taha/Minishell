/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 05:14:22 by marvin            #+#    #+#             */
/*   Updated: 2025/04/18 05:14:22 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function: is token type an operator
bool is_operator_token(enum e_type type)
{
	return (type == PIPE || type == AND_IF || type == OR_IF);
}

// Helper function: is token type a redirection
bool is_redir_token(enum e_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT ||
			type == APPEND || type == HEREDOC);
}

// Duplicate a command array (NULL-terminated)
char **dup_cmd_array(char **cmd)
{
	int i = 0;

	// Count elements
	while (cmd && cmd[i])
		i++;

	// Allocate new array
	char **new_cmd = malloc((i + 1) * sizeof(char *));
	if (!new_cmd)
		return (NULL);

	// Copy elements
	i = 0;
	while (cmd && cmd[i])
	{
		new_cmd[i] = strdup(cmd[i]);
		if (!new_cmd[i])
		{
			// Free what was allocated on failure
			while (--i >= 0)
				free(new_cmd[i]);
			free(new_cmd);
			return (NULL);
		}
		i++;
	}
	new_cmd[i] = NULL;

	return (new_cmd);
}

// Create a new tree node
t_tree *new_tree_node(enum e_type type, void *content)
{
	t_tree *node = malloc(sizeof(t_tree));
	if (!node)
		return (NULL);

	node->type = type;
	node->content = content;
	node->left = NULL;
	node->right = NULL;

	return (node);
}

// Add a node to the tree
void add_tree_node(t_tree **root, t_tree *node, char side)
{
	if (!root || !node)
		return;

	if (!(*root))
	{
		*root = node;
		return;
	}

	if (side == 'L')
		(*root)->left = node;
	else if (side == 'R')
		(*root)->right = node;
}

// Create a new redirection node
t_redir *new_redir_node(char *file, enum e_type type)
{
	t_redir *node = malloc(sizeof(t_redir));
	if (!node)
		return (NULL);

	node->file = strdup(file);
	if (!node->file)
	{
		free(node);
		return (NULL);
	}

	node->type = type;
	node->fd = -1;  // Initialize fd to -1 (not opened yet)
	node->next = NULL;

	return (node);
}

// Add a redirection node to a list
void add_redir_node(t_redir **lst, t_redir *new)
{
	t_redir *tmp;

	if (!lst || !new)
		return;

	if (!(*lst))
	{
		*lst = new;
		return;
	}

	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;

	tmp->next = new;
}

// Free redirection list
void free_redirs(t_redir **redirs)
{
	t_redir *tmp;

	if (!redirs || !(*redirs))
		return;

	while (*redirs)
	{
		tmp = (*redirs)->next;
		free((*redirs)->file);
		free(*redirs);
		*redirs = tmp;
	}
}

// Free the tree
void free_tree(t_tree **tree)
{
	if (!tree || !(*tree))
		return;

	// Free children first (post-order traversal)
	free_tree(&(*tree)->left);
	free_tree(&(*tree)->right);

	// Free content based on node type
	if ((*tree)->type == CMD && (*tree)->content)
	{
		t_cmd *cmd = (t_cmd *)(*tree)->content;

		// Free command array
		if (cmd->cmd)
		{
			for (int i = 0; cmd->cmd[i]; i++)
				free(cmd->cmd[i]);
			free(cmd->cmd);
		}

		// Free redirections
		free_redirs(&cmd->redir);

		// Free command struct
		free(cmd);
	}

	// Free the node itself
	free(*tree);
	*tree = NULL;
}

// Parse a simple command with its redirections
static t_tree *parse_command(t_token **tokens)
{
	t_cmd *cmd_content = malloc(sizeof(t_cmd));
	if (!cmd_content)
		return (NULL);

	// Initialize command structure
	cmd_content->cmd = NULL;
	cmd_content->redir = NULL;
	cmd_content->is_builtin = false; // Will be set during execution

	// Count the number of arguments
	int arg_count = 0;
	t_token *tmp = *tokens;

	while (tmp && tmp->type != PIPE && tmp->type != AND_IF && tmp->type != OR_IF)
	{
		// Skip whitespace
		if (tmp->type == SPACE || tmp->type == TAB || tmp->type == NEW_LINE)
		{
			tmp = tmp->next;
			continue;
		}

		// Handle redirections
		if (is_redir_token(tmp->type))
		{
			enum e_type redir_type = tmp->type;

			// Skip to the filename
			tmp = tmp->next;
			while (tmp && (tmp->type == SPACE || tmp->type == TAB))
				tmp = tmp->next;

			if (tmp && (tmp->type == CMD || tmp->type == ARG ||
						tmp->type == SQUOTE || tmp->type == DQUOTE))
			{
				// Add redirection
				add_redir_node(&cmd_content->redir,
							   new_redir_node(tmp->content, redir_type));
				tmp = tmp->next;
			}

			continue;
		}

		// Count arguments (CMD, ARG, quoted strings)
		if (tmp->type == CMD || tmp->type == ARG ||
			tmp->type == SQUOTE || tmp->type == DQUOTE)
			arg_count++;

		tmp = tmp->next;
	}

	// Allocate command array
	cmd_content->cmd = malloc((arg_count + 1) * sizeof(char *));
	if (!cmd_content->cmd)
	{
		free_redirs(&cmd_content->redir);
		free(cmd_content);
		return (NULL);
	}

	// Fill command array
	int i = 0;
	while (*tokens && (*tokens)->type != PIPE &&
		   (*tokens)->type != AND_IF && (*tokens)->type != OR_IF)
	{
		// Skip whitespace
		if ((*tokens)->type == SPACE || (*tokens)->type == TAB ||
			(*tokens)->type == NEW_LINE)
		{
			*tokens = (*tokens)->next;
			continue;
		}

		// Skip redirections and their filenames
		if (is_redir_token((*tokens)->type))
		{
			*tokens = (*tokens)->next;

			// Skip whitespace
			while (*tokens && ((*tokens)->type == SPACE || (*tokens)->type == TAB))
				*tokens = (*tokens)->next;

			// Skip filename
			if (*tokens)
				*tokens = (*tokens)->next;

			continue;
		}

		// Add argument
		if ((*tokens)->type == CMD || (*tokens)->type == ARG ||
			(*tokens)->type == SQUOTE || (*tokens)->type == DQUOTE)
		{
			cmd_content->cmd[i++] = strdup((*tokens)->content);
			*tokens = (*tokens)->next;
		}
		else
			*tokens = (*tokens)->next;
	}
	cmd_content->cmd[i] = NULL;

	// Create and return command node
	return (new_tree_node(CMD, cmd_content));
}

// Parse a pipeline
static t_tree *parse_pipeline(t_token **tokens)
{
	t_tree *left = parse_command(tokens);
	if (!left)
		return (NULL);

	// Skip whitespace
	while (*tokens && ((*tokens)->type == SPACE || (*tokens)->type == TAB))
		*tokens = (*tokens)->next;

	// No pipe, just return the command
	if (!*tokens || (*tokens)->type != PIPE)
		return (left);

	// Skip pipe token
	*tokens = (*tokens)->next;

	// Skip whitespace
	while (*tokens && ((*tokens)->type == SPACE || (*tokens)->type == TAB))
		*tokens = (*tokens)->next;

	// Parse right side of pipe
	t_tree *right = parse_pipeline(tokens);
	if (!right)
	{
		free_tree(&left);
		return (NULL);
	}

	// Create pipe node
	t_tree *pipe_node = new_tree_node(PIPE, NULL);
	if (!pipe_node)
	{
		free_tree(&left);
		free_tree(&right);
		return (NULL);
	}

	// Set children
	pipe_node->left = left;
	pipe_node->right = right;

	return (pipe_node);
}

// Parse logical expressions (AND_IF, OR_IF)
static t_tree *parse_logical(t_token **tokens)
{
	// Parse left side (pipeline)
	t_tree *left = parse_pipeline(tokens);
	if (!left)
		return (NULL);

	// Skip whitespace
	while (*tokens && ((*tokens)->type == SPACE || (*tokens)->type == TAB))
		*tokens = (*tokens)->next;

	// No logical operator, just return the pipeline
	if (!*tokens || ((*tokens)->type != AND_IF && (*tokens)->type != OR_IF))
		return (left);

	// Get operator type
	enum e_type op_type = (*tokens)->type;

	// Skip operator token
	*tokens = (*tokens)->next;

	// Skip whitespace
	while (*tokens && ((*tokens)->type == SPACE || (*tokens)->type == TAB))
		*tokens = (*tokens)->next;

	// Parse right side
	t_tree *right = parse_logical(tokens);
	if (!right)
	{
		free_tree(&left);
		return (NULL);
	}

	// Create operator node
	t_tree *op_node = new_tree_node(op_type, NULL);
	if (!op_node)
	{
		free_tree(&left);
		free_tree(&right);
		return (NULL);
	}

	// Set children
	op_node->left = left;
	op_node->right = right;

	return (op_node);
}

// Main parse function
t_tree *parse(t_token *tokens)
{
	// Make a copy of the tokens pointer so we can advance it
	t_token *tokens_ptr = tokens;

	// Parse the command line
	return parse_logical(&tokens_ptr);
}