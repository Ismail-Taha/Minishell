/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:45:47 by Ismail-Taha       #+#    #+#             */
/*   Updated: 2025/04/16 15:48:10 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Tests the lexer with different input strings
 */
void	test_lexer(void)
{
	t_token	*tokens;
	char	*test_inputs[] = {
		"echo hello world",
		"ls -la | grep .c",
		"echo \"quoted string\" 'another quote'",
		"cat file.txt > output.txt",
		"echo $HOME",
		"ls -l; pwd",
		"echo \"mixed $VAR in quotes\"",
		"cat << EOF",
		NULL
	};
	int		i;

	i = 0;
	while (test_inputs[i])
	{
		printf("\n\033[1;32mTesting: \"%s\"\033[0m\n", test_inputs[i]);
		tokens = tokenize(test_inputs[i]);
		if (tokens)
		{
			print_tokens(tokens);
			free_token_list(tokens);
		}
		else
			printf("Error tokenizing input\n");
		i++;
	}
}

/**
 * Main entry point - test our lexer
 */
int	main(void)
{
	printf("\033[1;34m===== MINISHELL LEXER TEST =====\033[0m\n");
	test_lexer();
	printf("\033[1;34m==============================\033[0m\n");
	return (0);
}