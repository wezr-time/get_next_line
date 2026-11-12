/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoubtas <ymoubtas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 09:20:16 by ymoubtas          #+#    #+#             */
/*   Updated: 2026/11/12 09:20:17 by ymoubtas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	index_of(const char *s, int c)
{
	unsigned char	ch;
	int				i;

	if (s == NULL)
		return (0);
	i = 0;
	ch = c;
	while (1)
	{
		if ((unsigned char)s[i] == ch)
			return (i);
		if ((unsigned char)s[i] == '\0')
			return (-1);
		i++;
	}
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	const unsigned char	*p_src;
	unsigned char		*p_dst;
	size_t				i;

	if (dest == src)
		return (dest);
	p_src = src;
	p_dst = dest;
	i = 0;
	while (i < n)
	{
		p_dst[i] = p_src[i];
		i++;
	}
	return (dest);
}

char	*free_s(char **s)
{
	free(*s);
	*s = NULL;
	return (NULL);
}
