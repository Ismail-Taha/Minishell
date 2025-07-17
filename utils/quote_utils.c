/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:30:13 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:30:14 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_quote_chars(const char *value, t_strip_ctx *ctx)
{
	if (!ctx->in_quotes)
	{
		ctx->in_quotes = 1;
		ctx->quote_type = value[ctx->i];
	}
	else if (value[ctx->i] == ctx->quote_type)
		ctx->in_quotes = 0;
	else
		(ctx->len_or_j)++;
}

static size_t	calculate_stripped_length(const char *value)
{
	t_strip_ctx	ctx;

	ctx.len_or_j = 0;
	ctx.i = 0;
	ctx.in_quotes = 0;
	while (value[ctx.i])
	{
		if ((value[ctx.i] == '"' || value[ctx.i] == '\''))
			process_quote_chars(value, &ctx);
		else
			ctx.len_or_j++;
		ctx.i++;
	}
	return (ctx.len_or_j);
}

static void	process_build_quote_chars(const char *value, t_strip_ctx *ctx)
{
	if (!ctx->in_quotes)
	{
		ctx->in_quotes = 1;
		ctx->quote_type = value[ctx->i];
	}
	else if (value[ctx->i] == ctx->quote_type)
		ctx->in_quotes = 0;
	else
		ctx->result[(ctx->len_or_j)++] = value[ctx->i];
}

static void	build_stripped_string(const char *value, char *result)
{
	t_strip_ctx	ctx;

	ctx.i = 0;
	ctx.len_or_j = 0;
	ctx.in_quotes = 0;
	ctx.result = result;
	while (value[ctx.i])
	{
		if ((value[ctx.i] == '"' || value[ctx.i] == '\''))
			process_build_quote_chars(value, &ctx);
		else
			result[ctx.len_or_j++] = value[ctx.i];
		ctx.i++;
	}
}

char	*strip_quotes(const char *value)
{
	size_t	len;
	char	*result;

	if (!value)
		return (NULL);
	len = calculate_stripped_length(value);
	if (len == ft_strlen(value))
	{
		result = ft_strdup(value);
		return (result);
	}
	result = ft_calloc(len + 1, 1);
	if (!result)
		return (NULL);
	build_stripped_string(value, result);
	return (result);
}
