/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_check_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:30:15 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:58:43 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_quote_check(t_quote_ctx *ctx)
{
	if (!*(ctx->in_quotes))
	{
		*(ctx->in_quotes) = 1;
		*(ctx->quote_type) = ctx->c;
		(*(ctx->i))++;
	}
	else if (*(ctx->in_quotes) && ctx->c == *(ctx->quote_type))
	{
		*(ctx->quoted) = 1;
		*(ctx->in_quotes) = 0;
		(*(ctx->i))++;
	}
	else
		(*(ctx->i))++;
}

int	is_content_quoted(char *content)
{
	size_t		i;
	int			in_quotes;
	char		quote_type;
	int			quoted;
	t_quote_ctx	ctx;

	i = 0;
	in_quotes = 0;
	quoted = 0;
	while (content[i])
	{
		if ((content[i] == '"' || content[i] == '\''))
		{
			ctx.c = content[i];
			ctx.i = &i;
			ctx.in_quotes = &in_quotes;
			ctx.quoted = &quoted;
			ctx.quote_type = &quote_type;
			process_quote_check(&ctx);
		}
		else
			i++;
	}
	return (quoted);
}
