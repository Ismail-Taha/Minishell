/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable_process.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:30:54 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:30:55 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	process_variable(t_var_expand *var_exp, char *str, char **envp)
{
	char	*var_name;
	char	*var_value;
	char	*temp;

	(var_exp->i)++;
	if (!str[var_exp->i] || ft_isspace(str[var_exp->i])
		|| str[var_exp->i] == '\"')
		return (add_char_to_result(&var_exp->result, '$'));
	var_name = extract_var_name(str, &var_exp->i);
	if (!var_name)
		return (0);
	var_value = get_env_value(var_name, envp);
	safe_free((void **)&var_name);
	if (!var_value)
	{
		var_exp->is_empty_var = 1;
		return (1);
	}
	temp = var_exp->result;
	var_exp->result = ft_strjoin(var_exp->result, var_value);
	safe_free((void **)&temp);
	safe_free((void **)&var_value);
	return (var_exp->result != NULL);
}

static int	handle_character(t_var_expand *var_exp, char *str, char **envp)
{
	if (str[var_exp->i] == '$' && str[var_exp->i + 1])
	{
		if (!process_variable(var_exp, str, envp))
			return (0);
	}
	else
	{
		if (!add_char_to_result(&var_exp->result, str[var_exp->i]))
		{
			safe_free((void **)&var_exp->result);
			return (0);
		}
		var_exp->i++;
	}
	return (1);
}

char	*expand_variables(char *str, char **envp)
{
	t_var_expand	var_exp;

	var_exp.i = 0;
	var_exp.is_empty_var = 0;
	var_exp.has_quoted_vars = has_var_in_dquotes(str);
	var_exp.result = ft_strdup("");
	if (!var_exp.result)
		return (NULL);
	while (str[var_exp.i])
	{
		if (!handle_character(&var_exp, str, envp))
		{
			safe_free((void **)&var_exp.result);
			return (NULL);
		}
	}
	if (var_exp.is_empty_var && var_exp.result && var_exp.result[0] == '\0'
		&& !var_exp.has_quoted_vars && !is_var_in_squotes(str))
		return (safe_free((void **)&var_exp.result), NULL);
	return (var_exp.result);
}
