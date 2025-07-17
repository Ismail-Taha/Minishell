/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_process.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:49:24 by isallali          #+#    #+#             */
/*   Updated: 2025/05/07 17:49:25 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	skip_whitespace(t_tokenizer *t)
{
	while (t->input[t->i] && ft_isspace(t->input[t->i]))
		(t->i)++;
	t->start = t->i;
}

t_token	*clean_tokens_return_null(t_token **tokens)
{
	ft_token_clear(tokens, free);
	return (NULL);
}

int	add_token(t_token **tokens, char *content)
{
	t_token	*new_token;

	new_token = ft_token_new(content);
	if (!new_token)
	{
		safe_free((void **)&content);
		return (0);
	}
	ft_token_add_back(tokens, new_token);
	return (1);
}

t_token	*tokenize_input(char *input)
{
	t_token		*tokens;
	t_tokenizer	t;

	tokens = NULL;
	t.tokens = &tokens;
	t.input = input;
	t.i = 0;
	t.start = 0;
	t.state = STATE_NORMAL;
	while (input[t.i])
	{
		if ((input[t.i] == '"' && t.state != STATE_IN_SINGLE_QUOTE)
			|| (input[t.i] == '\'' && t.state != STATE_IN_DOUBLE_QUOTE))
		{
			handle_quotes(&t);
			continue ;
		}
		if (t.state == STATE_NORMAL && !process_normal_char(&t))
			return (clean_tokens_return_null(&tokens));
		else if (t.state != STATE_NORMAL)
			t.i++;
	}
	if (!process_end_of_input(&t))
		return (clean_tokens_return_null(&tokens));
	return (tokens);
}
