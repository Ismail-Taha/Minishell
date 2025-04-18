#include "minishell.h"

int	is_operator(char c)
{
	return (c == '|' || c == '>' || c == '<');
}

char	*extract_operator_token(char *str, int *pos)
{
	int		start;
	char	*token;

	start = *pos;
	if ((str[*pos] == '>' || str[*pos] == '<') && (str[*pos] == str[*pos + 1]))
	{
		(*pos) += 2;
		token = ft_strndup(&str[start], 2);
	}
	else
	{
		(*pos)++;
		token = ft_strndup(&str[start], 1);
	}
	if (!token)
		fprintf(stderr, "minishell: memory allocation error\n");
	return (token);
}

void	assign_token_types(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (ft_strcmp(current->content, "|") == 0)
			current->type = TOKEN_PIPE;
		else if (ft_strcmp(current->content, ">") == 0)
			current->type = TOKEN_REDIRECT_OUT;
		else if (ft_strcmp(current->content, "<") == 0)
			current->type = TOKEN_REDIRECT_IN;
		else if (ft_strcmp(current->content, ">>") == 0)
			current->type = TOKEN_APPEND;
		else if (ft_strcmp(current->content, "<<") == 0)
			current->type = TOKEN_HEREDOC;
		else
			current->type = TOKEN_WORD;
		current = current->next;
	}
}
