#include "minishell.h"

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

char	*find_executable_path(char *cmd, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	if (!cmd || !*cmd)
		return (NULL);
	if ((cmd[0] == '/') || (cmd[0] == '.' && (cmd[1] == '/' || (cmd[1] == '.'
					&& cmd[2] == '/'))))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = get_path_env(envp);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	full_path = find_in_paths(paths, cmd);
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
	return (full_path);
}
