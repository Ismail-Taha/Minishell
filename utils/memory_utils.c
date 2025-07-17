/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:30:18 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:30:19 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	safe_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	safe_close(int *fd)
{
	if (fd && *fd >= 0)
	{
		close(*fd);
		*fd = -1;
	}
}

void	safe_doube_star_free(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		if (str[i])
			free(str[i]);
		i++;
	}
	free(str);
}

void	free_command(t_command *cmd)
{
	t_redirections	*redir;
	t_redirections	*next_redir;
	int				i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = -1;
		while (++i < cmd->args_count)
			safe_free((void **)&cmd->args[i]);
		safe_free((void **)&cmd->args);
	}
	redir = cmd->redirections;
	while (redir)
	{
		next_redir = redir->next;
		if (redir->type == TOKEN_HEREDOC && redir->heredoc_fd >= 0)
			safe_close(&redir->heredoc_fd);
		safe_free((void **)&redir->file);
		safe_free((void **)&redir);
		redir = next_redir;
	}
	safe_free((void **)&cmd);
}

void	free_command_list(t_command *cmd_list)
{
	t_command	*current;
	t_command	*next;

	current = cmd_list;
	while (current)
	{
		next = current->next;
		free_command(current);
		current = next;
	}
}
