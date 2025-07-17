/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env_convert_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:32:02 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:32:03 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_env_entries(t_env *env_list)
{
	t_env	*cur;
	int		size;

	size = 0;
	cur = env_list;
	while (cur)
	{
		if (cur->key)
			size++;
		cur = cur->next;
	}
	return (size);
}

static char	*create_env_string(char *key, char *value)
{
	char	*result;
	char	*temp;

	result = ft_strdup(key);
	if (!result)
		return (NULL);
	if (value)
	{
		temp = result;
		result = ft_strjoin(result, "=");
		safe_free((void **)&temp);
		if (!result)
			return (NULL);
		temp = result;
		result = ft_strjoin(result, value);
		safe_free((void **)&temp);
	}
	return (result);
}

char	**env_list_to_envp(t_env *env_list)
{
	t_env	*cur;
	char	**envp;
	int		size;
	int		i;

	size = count_env_entries(env_list);
	envp = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!envp)
		return (NULL);
	cur = env_list;
	i = 0;
	while (cur)
	{
		if (cur->key)
		{
			envp[i] = create_env_string(cur->key, cur->value);
			if (!envp[i])
				return (safe_doube_star_free(envp), NULL);
			i++;
		}
		cur = cur->next;
	}
	return (envp);
}

t_env	*envp_to_env_list(char **envp)
{
	t_env	*env_list;
	char	*equal_sign;
	char	*key;
	char	*value;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		if (equal_sign)
		{
			key = ft_substr(envp[i], 0, equal_sign - envp[i]);
			value = ft_strdup(equal_sign + 1);
			add_or_update_env(&env_list, key, value);
			safe_free((void **)&key);
			safe_free((void **)&value);
		}
		i++;
	}
	return (env_list);
}
