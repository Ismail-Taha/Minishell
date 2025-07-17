/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable_expand.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:30:57 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:30:58 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '?');
}

char	*extract_var_name(const char *str, int *pos)
{
	int	start;

	start = *pos;
	while (str[*pos] && is_var_char(str[*pos]))
		(*pos)++;
	return (ft_strndup(str + start, *pos - start));
}

char	*get_env_value(char *var_name, char **envp)
{
	int		i;
	size_t	var_len;
	char	*tmp;
	char	*interogation_var;

	i = 0;
	var_len = ft_strlen(var_name);
	if (ft_strncmp(var_name, "?", 1) == 0)
	{
		tmp = ft_itoa(g_last_exit_status);
		interogation_var = ft_strjoin(tmp, ft_strchr(var_name, '?') + 1);
		safe_free((void **)&tmp);
		return (interogation_var);
	}
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, var_len) == 0
			&& envp[i][var_len] == '=')
			return (ft_strdup(envp[i] + var_len + 1));
		i++;
	}
	return (NULL);
}

int	add_char_to_result(char **result, char c)
{
	char	*temp;
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	temp = *result;
	*result = ft_strjoin(*result, buf);
	if (temp)
		safe_free((void **)&temp);
	return (*result != NULL);
}

void	clean_empty_args(t_command *cmd)
{
	int	i;
	int	j;

	if (!cmd || !cmd->args)
		return ;
	i = 0;
	j = 0;
	while (i < cmd->args_count)
	{
		if (cmd->args[i] != NULL)
		{
			cmd->args[j] = cmd->args[i];
			if (i != j)
				cmd->args[i] = NULL;
			j++;
		}
		i++;
	}
	cmd->args_count = j;
}
