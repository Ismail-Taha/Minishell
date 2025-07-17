/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_quote_handling.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:30:59 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:31:00 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_quote_state(char c, t_state *state)
{
	if (c == '"' && *state != STATE_IN_SINGLE_QUOTE)
	{
		if (*state == STATE_NORMAL)
			*state = STATE_IN_DOUBLE_QUOTE;
		else
			*state = STATE_NORMAL;
	}
	else if (c == '\'' && *state != STATE_IN_DOUBLE_QUOTE)
	{
		if (*state == STATE_NORMAL)
			*state = STATE_IN_SINGLE_QUOTE;
		else
			*state = STATE_NORMAL;
	}
}

int	is_var_in_squotes(const char *str)
{
	int		i;
	t_state	state;

	i = 0;
	state = STATE_NORMAL;
	while (str[i])
	{
		update_quote_state(str[i], &state);
		if (str[i] == '$' && state == STATE_IN_SINGLE_QUOTE)
			return (1);
		i++;
	}
	return (0);
}

int	was_quoted(const char *str)
{
	int		in_quotes;
	char	quote_type;
	int		i;

	i = 0;
	in_quotes = 0;
	while (str && str[i])
	{
		if (!in_quotes && (str[i] == '\'' || str[i] == '"'))
		{
			in_quotes = 1;
			quote_type = str[i];
		}
		else if (in_quotes && str[i] == quote_type)
		{
			in_quotes = 0;
			return (1);
		}
		i++;
	}
	return (0);
}

int	has_var_in_dquotes(const char *str)
{
	int		i;
	t_state	state;

	i = 0;
	state = STATE_NORMAL;
	while (str[i])
	{
		update_quote_state(str[i], &state);
		if (str[i] == '$' && state == STATE_IN_DOUBLE_QUOTE)
			return (1);
		i++;
	}
	return (0);
}
