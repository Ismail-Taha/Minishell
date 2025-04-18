#include "minishell.h"

int	builtin_cd(t_command *cmd)
{
	char	*dir;
	char	*home;
	int		ret;

	if (cmd->args_count == 1)
	{
		home = getenv("HOME");
		if (!home)
		{
			fprintf(stderr, "minishell: cd: HOME not set\n");
			return (1);
		}
		dir = home;
	}
	else
		dir = cmd->args[1];
	ret = chdir(dir);
	if (ret != 0)
	{
		fprintf(stderr, "minishell: cd: %s: %s\n", dir, strerror(errno));
		return (1);
	}
	return (0);
}

int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		return (0);
	}
	return (1);
}
