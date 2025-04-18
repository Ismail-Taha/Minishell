#include "minishell.h"

char	*strip_quotes(const char *value)
{
	size_t	len;

	if (!value)
		return (NULL);
	len = ft_strlen(value);
	if (len >= 2 && value[0] == '"' && value[len - 1] == '"')
		return (ft_substr(value, 1, len - 2));
	return (ft_strdup(value));
}

void	export_one_arg(char *arg, t_env **env_list)
{
	char	*equal_sign;
	char	*key;
	char	*value;
	t_env	*node;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		key = ft_substr(arg, 0, equal_sign - arg);
		value = strip_quotes(equal_sign + 1);
	}
	else
	{
		key = ft_strdup(arg);
		node = find_env_node(*env_list, key);
		if (!node)
			value = ft_strdup("");
		else
			value = NULL;
	}
	if (!is_valid_identifier(key))
	{
		printf("minishell: export: `%s': not a valid identifier\n", key);
		free(key);
		if (value)
			free(value);
		return ;
	}
	add_or_update_env(env_list, key, value);
	free(key);
	if (value)
		free(value);
}
