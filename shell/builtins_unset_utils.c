#include "minishell.h"

void	unset_one_arg(char *arg, t_env **env_list)
{
	t_env	*prev;
	t_env	*cur;

	if (!is_valid_identifier(arg))
	{
		printf("minishell: unset: `%s': not a valid identifier\n", arg);
		return ;
	}
	prev = NULL;
	cur = *env_list;
	while (cur)
	{
		if (ft_strcmp(cur->key, arg) == 0)
		{
			if (prev)
				prev->next = cur->next;
			else
				*env_list = cur->next;
			free(cur->key);
			if (cur->value)
				free(cur->value);
			free(cur);
			break ;
		}
		prev = cur;
		cur = cur->next;
	}
}
