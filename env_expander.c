/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:14:07 by Ismail-Taha       #+#    #+#             */
/*   Updated: 2025/04/18 05:16:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "parsing.h"

// Forward declaration of environment list structure
struct s_env
{
	char *key;
	char *value;
	struct s_env *next;
};

// Get environment variable value
static char *get_env_value(char *var_name, t_env *env)
{
	t_env *tmp = env;

	while (tmp)
	{
		if (strcmp(tmp->key, var_name) == 0)
			return (strdup(tmp->value));

		tmp = tmp->next;
	}

	return (strdup(""));  // Return empty string if not found
}

// Expand variables in a string
static char *expand_string(char *str, t_env *env)
{
	if (!str || !*str)
		return (strdup(""));

	// Allocate initial buffer (can grow)
	int buffer_size = strlen(str) * 2;
	char *result = malloc(buffer_size * sizeof(char));
	if (!result)
		return (NULL);

	int i = 0;  // Input string position
	int r = 0;  // Result string position

	while (str[i])
	{
		// Check for variable
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ')
		{
			i++;  // Skip '$'

			// Extract variable name
			int var_start = i;
			while (str[i] && str[i] != ' ' && str[i] != '$' && str[i] != '"' && str[i] != '\'')
				i++;

			char *var_name = malloc((i - var_start + 1) * sizeof(char));
			if (!var_name)
			{
				free(result);
				return (NULL);
			}

			strncpy(var_name, str + var_start, i - var_start);
			var_name[i - var_start] = '\0';

			// Get variable value
			char *var_value = get_env_value(var_name, env);
			free(var_name);

			// Ensure result buffer is large enough
			int value_len = strlen(var_value);
			if (r + value_len >= buffer_size)
			{
				buffer_size = (r + value_len) * 2;
				char *new_result = realloc(result, buffer_size);
				if (!new_result)
				{
					free(result);
					free(var_value);
					return (NULL);
				}
				result = new_result;
			}

			// Copy variable value to result
			strcpy(result + r, var_value);
			r += value_len;
			free(var_value);
		}
		else
		{
			// Copy regular character
			result[r++] = str[i++];

			// Ensure result buffer is large enough
			if (r >= buffer_size - 1)
			{
				buffer_size *= 2;
				char *new_result = realloc(result, buffer_size);
				if (!new_result)
				{
					free(result);
					return (NULL);
				}
				result = new_result;
			}
		}
	}

	result[r] = '\0';
	return (result);
}

// Expand variables in command arguments
static void expand_cmd_args(t_cmd *cmd, t_env *env)
{
	if (!cmd || !cmd->cmd)
		return;

	for (int i = 0; cmd->cmd[i]; i++)
	{
		char *expanded = expand_string(cmd->cmd[i], env);
		if (expanded)
		{
			free(cmd->cmd[i]);
			cmd->cmd[i] = expanded;
		}
	}
}

// Expand variables in redirections
static void expand_redirs(t_redir *redir, t_env *env)
{
	t_redir *tmp = redir;

	while (tmp)
	{
		if (tmp->file && tmp->type != HEREDOC)  // Don't expand in heredoc delimiters
		{
			char *expanded = expand_string(tmp->file, env);
			if (expanded)
			{
				free(tmp->file);
				tmp->file = expanded;
			}
		}

		tmp = tmp->next;
	}
}

// Main function to expand environment variables in the AST
void expand_env_vars(t_tree *tree, t_env *env)
{
	if (!tree)
		return;

	// Handle command nodes
	if (tree->type == CMD && tree->content)
	{
		t_cmd *cmd = (t_cmd *)tree->content;
		expand_cmd_args(cmd, env);
		expand_redirs(cmd->redir, env);
	}

	// Process children recursively
	expand_env_vars(tree->left, env);
	expand_env_vars(tree->right, env);
}