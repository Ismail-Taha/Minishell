/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_types.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:49:41 by isallali          #+#    #+#             */
/*   Updated: 2025/05/07 17:49:42 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_operator(char c)
{
	return (c == '|' || c == '>' || c == '<');
}

char	*extract_operator_token(char *str, int *pos)
{
	int		start;
	char	*token;

	start = *pos;
	if ((str[*pos] == '>' || str[*pos] == '<') && (str[*pos] == str[*pos + 1]))
	{
		(*pos) += 2;
		token = ft_strndup(&str[start], 2);
	}
	else
	{
		(*pos)++;
		token = ft_strndup(&str[start], 1);
	}
	if (!token)
		ft_putstr_fd("minishell: memory allocation error\n", 2);
	return (token);
}

void	assign_token_types(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (ft_strcmp(current->content, "|") == 0)
			current->type = TOKEN_PIPE;
		else if (ft_strcmp(current->content, ">") == 0)
			current->type = TOKEN_REDIRECT_OUT;
		else if (ft_strcmp(current->content, "<") == 0)
			current->type = TOKEN_REDIRECT_IN;
		else if (ft_strcmp(current->content, ">>") == 0)
			current->type = TOKEN_APPEND;
		else if (ft_strcmp(current->content, "<<") == 0)
			current->type = TOKEN_HEREDOC;
		else
			current->type = TOKEN_WORD;
		current = current->next;
	}
}
