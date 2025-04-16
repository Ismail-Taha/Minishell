/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ismail-Taha                                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 04:14:07 by Ismail-Taha       #+#    #+#             */
/*   Updated: 2025/04/16 04:14:07 by Ismail-Taha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>

# include "lexer.h"

/* String utility functions (you'll need to implement these) */
char	*ft_strdup(const char *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_strlen(const char *s);
int		ft_strcmp(const char *s1, const char *s2);
char	**ft_split(char const *s, char c);

/* Main shell functions */
int		main_loop(char **env);
void	handle_signals(void);

#endif