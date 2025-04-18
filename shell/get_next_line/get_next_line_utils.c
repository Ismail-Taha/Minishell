/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:41:42 by yrachidi          #+#    #+#             */
/*   Updated: 2025/01/16 12:47:49 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*ft_calloc(size_t num_elements, size_t element_size)
{
	void	*ptr;
	size_t	total_size;
	size_t	i;

	if (num_elements != 0 && SIZE_MAX / num_elements < element_size)
		return (NULL);
	total_size = num_elements * element_size;
	if (total_size == 0)
		total_size = 1;
	ptr = malloc(total_size);
	if (!ptr)
		return (NULL);
	i = 0;
	while (total_size > i)
	{
		((unsigned char *)ptr)[i] = 0;
		i++;
	}
	return (ptr);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;
	size_t			i;

	i = 0;
	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	if (!dst && !src)
		return (NULL);
	while (i < n && d != s)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}




