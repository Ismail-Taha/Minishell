/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_redirect_setup.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:50:10 by isallali          #+#    #+#             */
/*   Updated: 2025/05/07 17:50:11 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_ambiguous_redirect(char *file_path, int was_in_squotes,
		int was_in_dquotes)
{
	if (was_in_squotes || was_in_dquotes)
		return (0);
	if (*file_path == '\0')
		return (1);
	while (file_path && *file_path)
	{
		if (ft_isspace((unsigned char)*file_path))
			return (1);
		file_path++;
	}
	return (0);
}

int	setup_redirect_in(char *file_path, int was_in_squotes, int was_in_dquotes)
{
	int	fd;

	if (is_ambiguous_redirect(file_path, was_in_squotes, was_in_dquotes))
		return (ft_fprintf_fd(2, "minishell: %s: ambiguous redirect\n",
				file_path), -1);
	fd = open(file_path, O_RDONLY);
	if (fd == -1)
		return (ft_fprintf_fd(2, "minishell: "), perror(file_path), -1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (ft_fprintf_fd(2, "minishell: input redirection failed: %s\n",
				strerror(errno)), close(fd), -1);
	close(fd);
	return (0);
}

int	setup_redirect_out(char *file_path, int append_mode, int was_in_squotes,
		int was_in_dquotes)
{
	int	flags;
	int	fd;

	if (is_ambiguous_redirect(file_path, was_in_squotes, was_in_dquotes))
		return (ft_fprintf_fd(2, "minishell: %s: ambiguous redirect\n",
				file_path), -1);
	flags = O_WRONLY | O_CREAT;
	if (append_mode)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(file_path, flags, 0644);
	if (fd == -1)
		return (ft_fprintf_fd(2, "minishell: "), perror(file_path), -1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (ft_fprintf_fd(2, "minishell: output redirection failed: %s\n",
				strerror(errno)), close(fd), -1);
	return (close(fd), 0);
}

static int	handle_single_redirection(t_redirections *redir)
{
	int	result;

	result = 0;
	if (redir->type == TOKEN_REDIRECT_IN)
		result = setup_redirect_in(redir->file, redir->was_in_squotes,
				redir->was_in_dquotes);
	else if (redir->type == TOKEN_REDIRECT_OUT)
		result = setup_redirect_out(redir->file, 0, redir->was_in_squotes,
				redir->was_in_dquotes);
	else if (redir->type == TOKEN_APPEND)
		result = setup_redirect_out(redir->file, 1, redir->was_in_squotes,
				redir->was_in_dquotes);
	else if (redir->type == TOKEN_HEREDOC)
	{
		if (dup2(redir->heredoc_fd, STDIN_FILENO) == -1)
			return (ft_fprintf_fd(2, "minishell: heredoc redirection "),
				ft_fprintf_fd(2, "failed: %s\n", strerror(errno)) - 1);
		safe_close(&redir->heredoc_fd);
	}
	else
	{
		ft_putstr_fd("minishell: unknown redirection type\n", 2);
		result = -1;
	}
	return (result);
}

int	setup_redirections(t_command *cmd)
{
	t_redirections	*redir;
	int				result;

	redir = cmd->redirections;
	result = 0;
	while (redir && result == 0)
	{
		result = handle_single_redirection(redir);
		redir = redir->next;
	}
	return (result);
}
