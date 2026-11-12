/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoubtas <ymoubtas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 09:20:07 by ymoubtas          #+#    #+#             */
/*   Updated: 2026/11/12 09:20:08 by ymoubtas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

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

char	*free_node(t_list **holder, int fd)
{
	t_list	*tmp;

	while (*holder)
	{
		if ((*holder)->fd == fd)
		{
			tmp = *holder;
			*holder = tmp->next;
			free(tmp->content);
			free(tmp);
			break ;
		}
		holder = &(*holder)->next;
	}
	return (NULL);
}

t_list	*init(t_list **holder, int fd)
{
	t_list	*node;

	while (*holder && (*holder)->fd != fd)
		holder = &(*holder)->next;
	if (!*holder)
	{
		node = malloc(sizeof(t_list));
		if (!node)
			return (NULL);
		node->fd = fd;
		node->content = NULL;
		node->next = NULL;
		*holder = node;
	}
	return (*holder);
}

char	*join(char **s1, char *s2, ssize_t s2_size)
{
	char	*new_holder;
	int		holder_len;

	if (s2_size == 0)
		return (NULL);
	holder_len = index_of(*s1, '\0');
	new_holder = malloc(holder_len + s2_size + 1);
	if (!new_holder)
		return (NULL);
	if (*s1)
		ft_memcpy(new_holder, *s1, holder_len);
	ft_memcpy(new_holder + holder_len, s2, s2_size);
	new_holder[holder_len + s2_size] = '\0';
	free(*s1);
	*s1 = new_holder;
	return (new_holder);
}
