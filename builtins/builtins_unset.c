/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:32:44 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 19:21:08 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_env_node(t_env *node)
{
	safe_free((void **)&node->key);
	if (node->value)
		safe_free((void **)&node->value);
	safe_free((void **)&node);
}

static void	remove_env_node(t_env **env_list, t_env *prev, t_env *cur)
{
	if (prev)
		prev->next = cur->next;
	else
		*env_list = cur->next;
	free_env_node(cur);
}

void	unset_one_arg(char *arg, t_env **env_list)
{
	t_env	*prev;
	t_env	*cur;

	if (!is_valid_identifier(arg))
	{
		ft_fprintf_fd(2, "minishell: unset: `%s': not a valid identifier\n",
			arg);
		return ;
	}
	prev = NULL;
	cur = *env_list;
	while (cur)
	{
		if (ft_strcmp(cur->key, arg) == 0)
		{
			remove_env_node(env_list, prev, cur);
			break ;
		}
		prev = cur;
		cur = cur->next;
	}
}

int	builtin_unset(t_command *cmd, t_env **env_list)
{
	int	i;

	i = 1;
	while (i < cmd->args_count)
	{
		unset_one_arg(cmd->args[i], env_list);
		i++;
	}
	return (0);
}
