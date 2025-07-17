/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_arg_processing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:31:09 by yrachidi          #+#    #+#             */
/*   Updated: 2025/05/07 17:31:10 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	expand_and_strip_arg(t_command *cmd, char **envp, int i)
{
	char	*expanded;
	char	*stripped;

	if (!cmd->args[i])
		return ;
	expanded = expand_variables(cmd->args[i], envp);
	safe_free((void **)&cmd->args[i]);
	if (!expanded)
	{
		cmd->args[i] = NULL;
		return ;
	}
	cmd->args[i] = expanded;
	stripped = strip_quotes(cmd->args[i]);
	if (stripped)
	{
		safe_free((void **)&cmd->args[i]);
		cmd->args[i] = stripped;
	}
}

static void	process_arg_expansion(t_expand_vars *v)
{
	char	*stripped;

	v->was_arg_quoted = was_quoted(v->cmd->args[v->i]);
	v->had_quoted_vars = has_var_in_dquotes(v->cmd->args[v->i]);
	if (!is_var_in_squotes(v->cmd->args[v->i]))
		expand_and_strip_arg(v->cmd, v->envp, v->i);
	else
	{
		stripped = strip_quotes(v->cmd->args[v->i]);
		if (stripped)
		{
			safe_free((void **)&v->cmd->args[v->i]);
			v->cmd->args[v->i] = stripped;
		}
	}
}

void	expand_args_loop(t_expand_vars *v)
{
	if (!v->cmd || !v->cmd->args || v->cmd->args_count <= 0)
		return ;
	v->i = 0;
	while (v->i < v->cmd->args_count)
	{
		if (!v->cmd->args[v->i])
		{
			v->i++;
			continue ;
		}
		process_arg_expansion(v);
		if (v->i > 0 || v->cmd->args_count == 1)
		{
			if (!v->was_arg_quoted && !v->had_quoted_vars)
			{
				v->added = split_and_insert_args(v);
				if (v->added > 0)
					v->i += v->added - 1;
			}
		}
		v->i++;
	}
}

void	expand_redirections_loop(t_expand_vars *v)
{
	t_redirections	*redir;
	char			*expanded;

	redir = v->cmd->redirections;
	while (redir)
	{
		if (redir->type != TOKEN_HEREDOC && !redir->was_in_squotes)
		{
			expanded = expand_variables(redir->file, v->envp);
			if (expanded)
			{
				safe_free((void **)&redir->file);
				redir->file = expanded;
			}
		}
		redir = redir->next;
	}
}
