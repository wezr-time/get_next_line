/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoubtas <ymoubtas@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 09:19:53 by ymoubtas          #+#    #+#             */
/*   Updated: 2026/11/12 09:19:54 by ymoubtas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*read_buf_to_holder(t_list *holder, int fd)
{
	char	*buf;
	ssize_t	bytes_read;

	if (holder->content && index_of(holder->content, '\n') > -1)
		return (holder->content);
	buf = malloc((size_t)BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	bytes_read = read(fd, buf, (size_t)BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buf[bytes_read] = '\0';
		if (join(&holder->content, buf, bytes_read) == NULL)
		{
			free(buf);
			return (NULL);
		}
		if (index_of(holder->content, '\n') > -1)
			break ;
		bytes_read = read(fd, buf, (size_t)BUFFER_SIZE);
	}
	free(buf);
	if (bytes_read < 0 || (bytes_read == 0 && !(holder->content)))
		return (NULL);
	return (holder->content);
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
	join(&new_holder, *holder + holder_len, total);
	free(*holder);
	*holder = new_holder;
	return (new_holder);
}

char	*get_next_line(int fd)
{
	static t_list	*holder;
	t_list			*node;
	char			*line;

	if (fd < 0 || (size_t)BUFFER_SIZE <= 0)
		return (NULL);
	node = init(&holder, fd);
	if (node == NULL)
		return (NULL);
	if (read_buf_to_holder(node, fd) == NULL)
		return (free_node(&holder, fd));
	line = update_line(node->content);
	if (!line)
		return (free_node(&holder, fd));
	if (update_holder(&node->content) == NULL)
		free_node(&holder, fd);
	if (line)
		return (line);
	return (NULL);
}
