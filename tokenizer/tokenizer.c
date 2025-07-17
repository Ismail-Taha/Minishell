/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:49:08 by isallali          #+#    #+#             */
/*   Updated: 2025/05/07 17:49:09 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	process_operator(t_tokenizer *t)
{
	char	*token_content;

	if (t->i > t->start)
	{
		token_content = extract_word_token(t->input, t->start, t->i);
		if (!token_content || !add_token(t->tokens, token_content))
			return (0);
	}
	token_content = extract_operator_token(t->input, &t->i);
	if (!token_content || !add_token(t->tokens, token_content))
		return (0);
	t->start = t->i;
	return (1);
}

int	process_whitespace(t_tokenizer *t)
{
	char	*token_content;

	if (t->i > t->start)
	{
		token_content = extract_word_token(t->input, t->start, t->i);
		if (!token_content || !add_token(t->tokens, token_content))
			return (0);
	}
	skip_whitespace(t);
	return (1);
}

int	process_normal_char(t_tokenizer *t)
{
	if (is_operator(t->input[t->i]))
		return (process_operator(t));
	else if (ft_isspace(t->input[t->i]))
		return (process_whitespace(t));
	(t->i)++;
	return (1);
}

int	process_end_of_input(t_tokenizer *t)
{
	char	*token_content;

	if (t->i > t->start)
	{
		token_content = extract_word_token(t->input, t->start, t->i);
		if (!token_content || !add_token(t->tokens, token_content))
			return (0);
	}
	if (t->state != STATE_NORMAL)
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
		return (0);
	}
	return (1);
}

void	handle_quotes(t_tokenizer *t)
{
	char	quote_char;

	quote_char = t->input[t->i];
	if (t->state == STATE_NORMAL)
	{
		if (quote_char == '\'')
			t->state = STATE_IN_SINGLE_QUOTE;
		else
			t->state = STATE_IN_DOUBLE_QUOTE;
	}
	else if ((t->state == STATE_IN_SINGLE_QUOTE && quote_char == '\'')
		|| (t->state == STATE_IN_DOUBLE_QUOTE && quote_char == '"'))
		t->state = STATE_NORMAL;
	(t->i)++;
}
