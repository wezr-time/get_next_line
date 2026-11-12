/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoubtas <ymoubtas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 09:20:21 by ymoubtas          #+#    #+#             */
/*   Updated: 2026/11/12 09:20:22 by ymoubtas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*join(char **s1, char *s2, ssize_t s2_size)
{
	char	*new_holder;
	int		holder_len;

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

static char	*read_buf_to_holder(char **holder, int fd)
{
	char	*buf;
	ssize_t	bytes_read;

	if (*holder && index_of(*holder, '\n') > -1)
		return (*holder);
	buf = malloc((size_t)BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	bytes_read = read(fd, buf, (size_t)BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buf[bytes_read] = '\0';
		if (join(holder, buf, bytes_read) == NULL)
		{
			free(buf);
			return (NULL);
		}
		if (index_of(*holder, '\n') > -1)
			break ;
		bytes_read = read(fd, buf, (size_t)BUFFER_SIZE);
	}
	free(buf);
	if (bytes_read < 0 || (bytes_read == 0 && !(*holder)))
		return (NULL);
	return (*holder);
}

static char	*update_line(char *holder)
{
	char	*new_line;
	int		line_len;

	line_len = index_of(holder, '\n');
	if (line_len == -1)
		line_len = index_of(holder, '\0');
	else
		line_len++;
	new_line = NULL;
	new_line = join(&new_line, holder, line_len);
	return (new_line);
}

static char	*update_holder(char **holder)
{
	char	*new_holder;
	int		holder_len;
	int		total;

	holder_len = index_of(*holder, '\n');
	if (holder_len == -1)
		return (NULL);
	holder_len++;
	new_holder = NULL;
	total = index_of(*holder, '\0') - holder_len;
	if (total != 0)
		join(&new_holder, *holder + holder_len, total);
	free(*holder);
	*holder = new_holder;
	return (new_holder);
}

char	*get_next_line(int fd)
{
	static char	*holder = NULL;
	char		*line;

	if (fd < 0 || (size_t)BUFFER_SIZE <= 0)
		return (NULL);
	if (read_buf_to_holder(&holder, fd) == NULL)
		return (free_s(&holder));
	line = update_line(holder);
	if (!line)
		return (free_s(&holder));
	if (update_holder(&holder) == NULL)
		free_s(&holder);
	if (line)
		return (line);
	return (NULL);
}
