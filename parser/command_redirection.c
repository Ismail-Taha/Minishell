/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:49:58 by isallali          #+#    #+#             */
/*   Updated: 2025/05/07 17:49:59 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	create_cmd_if_needed(t_command **first_cmd, t_command **current_cmd)
{
	t_command	*new_cmd;

	if (!*current_cmd)
	{
		new_cmd = command_init();
		if (!new_cmd)
			return (0);
		if (!*first_cmd)
			*first_cmd = new_cmd;
		*current_cmd = new_cmd;
	}
	return (1);
}

static int	check_redirect_syntax(t_token **current)
{
	if (!*current || (*current)->type != TOKEN_WORD)
	{
		if (!*current)
		{
			ft_fprintf_fd(2, "minishell: syntax error near ");
			ft_fprintf_fd(2, "unexpected token `newline'\n");
		}
		else
		{
			ft_fprintf_fd(2, "minishell: syntax error near ");
			ft_fprintf_fd(2, "unexpected token %s\n", (*current)->content);
		}
		g_last_exit_status = 2;
		return (0);
	}
	return (1);
}

int	handle_redirect_token(t_token **current, t_command **first_cmd,
		t_command **current_cmd)
{
	t_token_type	redirect_type;

	if (!create_cmd_if_needed(first_cmd, current_cmd))
		return (0);
	redirect_type = (*current)->type;
	*current = (*current)->next;
	if (!check_redirect_syntax(current))
		return (0);
	add_redirection(*current_cmd, redirect_type, (*current)->content);
	*current = (*current)->next;
	if (*current && (*current)->type == TOKEN_WORD)
		add_words_as_args(*current_cmd, current);
	return (1);
}
