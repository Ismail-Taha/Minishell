/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:32:35 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:32:36 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_append_mode(char **value, char *key, t_env **env_list)
{
	t_env	*existing;
	char	*new_value;

	existing = find_env_node(*env_list, key);
	if (existing && existing->value && *value)
	{
		new_value = ft_strjoin(existing->value, *value);
		*value = new_value;
	}
}

static void	handle_error(char *key, char **value)
{
	ft_fprintf_fd(2, "minishell: export: `%s': not a valid identifier\n", key);
	safe_free((void **)&key);
	if (*value)
		safe_free((void **)value);
}

static void	extract_key_value(char *arg, char **key, char **value,
		int *append_mode)
{
	char	*equal_sign;

	*append_mode = 0;
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		if (equal_sign > arg && *(equal_sign - 1) == '+')
		{
			*append_mode = 1;
			*key = ft_substr(arg, 0, equal_sign - arg - 1);
		}
		else
			*key = ft_substr(arg, 0, equal_sign - arg);
		*value = strip_quotes(equal_sign + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
}

int	export_one_arg(char *arg, t_env **env_list)
{
	char	*key;
	char	*value;
	int		append_mode;

	extract_key_value(arg, &key, &value, &append_mode);
	if (!is_valid_identifier(key))
	{
		handle_error(key, &value);
		return (0);
	}
	if (append_mode)
		handle_append_mode(&value, key, env_list);
	add_or_update_env(env_list, key, value);
	safe_free((void **)&key);
	if (value)
		safe_free((void **)&value);
	return (1);
}
