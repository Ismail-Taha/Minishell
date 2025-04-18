#include "minishell.h"

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
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

t_env	*add_or_update_env(t_env **env_list, const char *key, const char *value)
{
	t_env	*node;

	node = find_env_node(*env_list, key);
	if (node)
	{
		if (node->value)
			free(node->value);
		if (value != NULL)
			node->value = ft_strdup(value);
		else
			node->value = NULL;
		return (node);
	}
	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (value != NULL)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	node->next = *env_list;
	*env_list = node;
	return (node);
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
			free(key);
			free(value);
		}
		i++;
	}
	return (env_list);
}

void	print_export_list(t_env *env_list)
{
	t_env	*cur;

	cur = env_list;
	while (cur)
	{
		if (cur->key)
			printf("declare -x %s=\"%s\"\n", cur->key,
				cur->value ? cur->value : "");
		cur = cur->next;
	}
}

char	**env_list_to_envp(t_env *env_list)
{
	int		size;
	int		i;
	t_env	*cur;
	char	**envp;
	char	*tmp;

	size = 0;
	cur = env_list;
	while (cur)
	{
		if (cur->key)
			size++;
		cur = cur->next;
	}
	envp = (char **)malloc(sizeof(char *) * (size + 1));
	if (!envp)
		return (NULL);
	cur = env_list;
	i = 0;
	while (cur)
	{
		if (cur->key)
		{
			if (cur->value)
			{
				tmp = malloc(ft_strlen(cur->key) + ft_strlen(cur->value) + 2);
				if (tmp)
				{
					ft_strlcpy(tmp, cur->key, ft_strlen(cur->key) + 1);
					ft_strlcat(tmp, "=", ft_strlen(cur->key) + 2);
					ft_strlcat(tmp, cur->value, ft_strlen(cur->key)
						+ ft_strlen(cur->value) + 2);
					envp[i++] = tmp;
				}
			}
			else
			{
				tmp = malloc(ft_strlen(cur->key) + 2);
				if (tmp)
				{
					ft_strlcpy(tmp, cur->key, ft_strlen(cur->key) + 1);
					ft_strlcat(tmp, "=", ft_strlen(cur->key) + 2);
					envp[i++] = tmp;
				}
			}
		}
		cur = cur->next;
	}
	envp[i] = NULL;
	return (envp);
}
