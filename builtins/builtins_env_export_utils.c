/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env_export_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:32:04 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:32:05 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_') || ft_strchr(str, '+')
		|| ft_strchr(str, '-'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

t_env	*find_env_node(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

static t_env	*create_env_node(const char *key, const char *value)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (value != NULL)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	node->next = NULL;
	if (!node->key || (value != NULL && !node->value))
	{
		safe_free((void **)&node->key);
		safe_free((void **)&node->value);
		safe_free((void **)&node);
		return (NULL);
	}
	return (node);
}

static void	append_env_node(t_env **env_list, t_env *node)
{
	t_env	*last;

	if (*env_list == NULL)
		*env_list = node;
	else
	{
		last = *env_list;
		while (last->next)
			last = last->next;
		last->next = node;
	}
}

void	add_or_update_env(t_env **env_list, const char *key, const char *value)
{
	t_env	*node;

	if (!key || !env_list)
		return ;
	node = find_env_node(*env_list, key);
	if (node)
	{
		safe_free((void **)&node->value);
		if (value != NULL)
			node->value = ft_strdup(value);
		else
			node->value = NULL;
		return ;
	}
	node = create_env_node(key, value);
	if (!node)
		return ;
	append_env_node(env_list, node);
}
