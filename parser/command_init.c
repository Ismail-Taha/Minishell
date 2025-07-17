/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:50:26 by isallali          #+#    #+#             */
/*   Updated: 2025/05/07 17:50:27 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command	*command_init(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->args_count = 0;
	cmd->redirections = NULL;
	cmd->next = NULL;
	return (cmd);
}

static int	allocate_command_args(t_command *cmd, t_token **tokens)
{
	int		arg_count;
	t_token	*current;

	arg_count = 0;
	current = *tokens;
	while (current && current->type == TOKEN_WORD)
	{
		arg_count++;
		current = current->next;
	}
	cmd->args = ft_calloc(arg_count + 1, sizeof(char *));
	if (!cmd->args)
		return (0);
	cmd->args_count = arg_count;
	return (1);
}

static void	copy_args_to_command(t_command *cmd, t_token **tokens)
{
	int		i;
	t_token	*current;

	i = 0;
	current = *tokens;
	while (i < cmd->args_count && current && current->type == TOKEN_WORD)
	{
		cmd->args[i] = ft_strdup(current->content);
		i++;
		current = current->next;
	}
	cmd->args[i] = NULL;
	*tokens = current;
}

t_command	*create_command_type_word(t_token **tokens)
{
	t_command	*cmds;

	if (!*tokens || (*tokens)->type != TOKEN_WORD)
		return (NULL);
	cmds = command_init();
	if (!cmds)
		return (NULL);
	if (!allocate_command_args(cmds, tokens))
	{
		free_command(cmds);
		return (NULL);
	}
	copy_args_to_command(cmds, tokens);
	return (cmds);
}

t_command	*finish_command_parsing(t_command *first_cmd)
{
	t_command	*current;

	current = first_cmd;
	while (current)
	{
		if (!current->args)
		{
			current->args = malloc(sizeof(char *));
			if (!current->args)
				return (free_command_list(first_cmd), NULL);
			current->args[0] = NULL;
			current->args_count = 0;
		}
		current = current->next;
	}
	return (first_cmd);
}
