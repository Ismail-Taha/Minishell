/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:51:57 by yrachidi          #+#    #+#             */
/*   Updated: 2024/10/31 13:52:58 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_words_count(char const *s, char c)
{
	int	count;
	int	word;

	word = 0;
	count = 0;
	while (*s)
	{
		if (*s != c && !word)
		{
			word = 1;
			count++;
		}
		else if (*s == c)
			word = 0;
		s++;
	}
	return (count);
}

static char	*ft_str_s_dup(char const *s, char c, int *start)
{
	char	*dup;
	int		end;
	int		i;

	end = 0;
	while (s[*start + end] && s[*start + end] != c)
		end++;
	dup = ft_calloc(end + 1, 1);
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (i < end)
	{
		dup[i] = s[*start];
		(*start)++;
		i++;
	}
	return (dup);
}

void	ft_free_strs(char **strs)
{
	int	i;

	i = 0;
	while (strs[i] != NULL)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	int		i;
	int		j;

	if (!s)
		return (NULL);
	strs = ft_calloc((ft_words_count(s, c) + 1), sizeof(char *));
	if (strs == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] == c && s[i++])
			continue ;
		strs[j] = ft_str_s_dup(s, c, &i);
		if (strs[j++] == NULL)
			return (ft_free_strs(strs), NULL);
	}
	return (strs[j] = NULL, strs);
}
