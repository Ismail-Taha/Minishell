/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_redirection_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:50:03 by isallali          #+#    #+#             */
/*   Updated: 2025/05/07 19:28:32 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_redirection(t_command *cmd, t_token_type redirect_type, char *file)
{
	t_redirections	*redirection;
	t_redirections	*current;

	redirection = malloc(sizeof(t_redirections));
	if (!redirection)
		return ;
	redirection->type = redirect_type;
	redirection->was_in_squotes = is_var_in_squotes(file);
	redirection->was_in_dquotes = has_var_in_dquotes(file);
	file = strip_quotes(file);
	redirection->file = ft_strdup(file);
	safe_free((void **)&file);
	redirection->heredoc_fd = -1;
	redirection->next = NULL;
	if (!redirection->file)
		return (safe_free((void **)&redirection));
	if (!cmd->redirections)
		cmd->redirections = redirection;
	else
	{
		current = cmd->redirections;
		while (current->next)
			current = current->next;
		current->next = redirection;
	}
}

int	count_word_tokens(t_token *current)
{
	int		new_count;
	t_token	*temp;

	new_count = 0;
	temp = current;
	while (temp && temp->type == TOKEN_WORD)
	{
		new_count++;
		temp = temp->next;
	}
	return (new_count);
}

char	**allocate_args_array(t_command *cmd, int new_count)
{
	char	**new_args;
	int		i;

	new_args = malloc(sizeof(char *) * (cmd->args_count + new_count + 1));
	if (!new_args)
		return (NULL);
	i = 0;
	while (i < cmd->args_count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	return (new_args);
}

int	add_word_to_args(t_token **current, int i, char **new_args)
{
	new_args[i] = ft_strdup((*current)->content);
	if (!new_args[i])
		return (0);
	*current = (*current)->next;
	return (1);
}

void	add_words_as_args(t_command *cmd, t_token **current)
{
	int		new_count;
	char	**new_args;
	int		i;
	int		j;

	new_count = count_word_tokens(*current);
	if (new_count == 0)
		return ;
	new_args = allocate_args_array(cmd, new_count);
	if (!new_args)
		return ;
	i = cmd->args_count;
	j = 0;
	while (*current && (*current)->type == TOKEN_WORD && j < new_count)
	{
		add_word_to_args(current, i + j, new_args);
		j++;
	}
	new_args[i + j] = NULL;
	safe_free((void **)&cmd->args);
	cmd->args = new_args;
	cmd->args_count += new_count;
}
