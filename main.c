/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ismail-Taha                                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:14:07 by Ismail-Taha       #+#    #+#             */
/*   Updated: 2025/04/16 04:14:07 by Ismail-Taha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Signal handler setup
 */
void	handle_signals(void)
{
	// TODO: Implement signal handling for CTRL+C, CTRL+D, etc.
}

/**
 * Main shell loop
 * 
 * @param env Environment variables
 * @return Exit status
 */
int	main_loop(char **env)
{
	char	*input;
	t_token	*tokens;
	int		status;

	status = 0;
	while (1)
	{
		// Display prompt and get input
		input = readline("minishell$ ");
		if (!input) // EOF (CTRL+D)
			break;
		
		// Add to history if not empty
		if (*input)
			add_history(input);

		// Tokenize input
		tokens = tokenize(input);
		if (tokens)
		{
			// For debugging
			print_tokens(tokens);

			// TODO: Add parser and execution here
			
			// Free tokens
			free_token_list(tokens);
		}
		else
			printf("Error: Tokenization failed\n");

		// Free input
		free(input);
	}
	return (status);
}

/**
 * Main function
 * 
 * @param argc Argument count
 * @param argv Argument vector
 * @param env Environment variables
 * @return Exit status
 */
int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;

	// Set up signal handling
	handle_signals();

	// Enter main shell loop
	return (main_loop(env));
}