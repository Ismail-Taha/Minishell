/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isallali <isallali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:50:17 by isallali          #+#    #+#             */
/*   Updated: 2025/05/07 17:50:18 by isallali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_path_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*find_in_paths(char **paths, char *cmd)
{
	int		i;
	char	*full_path;
	char	*temp;

	i = 0;
	full_path = NULL;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		if (!full_path)
			break ;
		temp = full_path;
		full_path = ft_strjoin(full_path, cmd);
		free(temp);
		if (!full_path)
			break ;
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		full_path = NULL;
		i++;
	}
	return (NULL);
}

int	is_path_with_slash(char *cmd)
{
	return (cmd[0] == '/' || (cmd[0] == '.' && (cmd[1] == '/' || (cmd[1] == '.'
					&& cmd[2] == '/'))));
}

char	*check_direct_path(char *cmd)
{
	if (is_path_with_slash(cmd))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
	}
	return (NULL);
}

char	*find_executable_path(char *cmd, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*full_path;

	if (!cmd || !*cmd)
		return (NULL);
	full_path = check_direct_path(cmd);
	if (full_path)
		return (full_path);
	path_env = get_path_env(envp);
	if (!path_env)
		return (safe_free((void **)full_path), NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	full_path = find_in_paths(paths, cmd);
	safe_doube_star_free(paths);
	return (full_path);
}
