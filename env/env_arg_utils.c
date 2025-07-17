/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_arg_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:31:05 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:31:06 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	copy_and_replace_args(t_command *cmd, char **new_args, int pos,
		char **split_words)
{
	int	i;
	int	j;
	int	k;
	int	l;

	i = -1;
	while (++i < pos)
		new_args[i] = cmd->args[i];
	j = -1;
	while (split_words[++j])
		new_args[i + j] = ft_strdup(split_words[j]);
	k = pos + 1;
	while (k < cmd->args_count)
	{
		new_args[i + j] = cmd->args[k];
		k++;
		j++;
	}
	new_args[i + j] = NULL;
	safe_free((void **)&cmd->args[pos]);
	l = -1;
	while (split_words[++l])
		safe_free((void **)&split_words[l]);
	safe_free((void **)&split_words);
}

int	add_split_args_to_command(t_command *cmd, int pos, char **split_words)
{
	int		word_count;
	char	**new_args;
	int		new_size;

	word_count = count_split_words(split_words);
	if (word_count == 0)
		return (0);
	new_size = cmd->args_count + word_count - 1;
	new_args = ft_calloc(new_size + 1, sizeof(char *));
	if (!new_args)
		return (0);
	copy_and_replace_args(cmd, new_args, pos, split_words);
	safe_free((void **)&cmd->args);
	cmd->args = new_args;
	cmd->args_count = new_size;
	return (word_count);
}

static void	free_split_words(char **split_words)
{
	int	i;

	i = 0;
	if (!split_words || !*split_words)
		return ;
	while (split_words[i])
	{
		safe_free((void **)&split_words[i]);
		i++;
	}
	safe_free((void **)&split_words);
}

int	split_and_insert_args(t_expand_vars *v)
{
	char	**split_words;
	int		added;

	if ((v->i > 0 && !v->is_export) || (v->i == 0 && v->cmd->args_count == 1))
	{
		split_words = ft_split(v->cmd->args[v->i], ' ');
		if (!split_words || !split_words[0])
		{
			free_split_words(split_words);
			return (0);
		}
		if (count_split_words(split_words) > 1 || v->i > 0)
		{
			added = add_split_args_to_command(v->cmd, v->i, split_words);
			if (added > 0)
				return (added);
		}
		free_split_words(split_words);
	}
	return (0);
}
