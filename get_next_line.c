/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 21:29:06 by acoezard          #+#    #+#             */
/*   Updated: 2021/10/20 11:41:22 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*gnl_get_line(char *buffer)
{
	char	*line;
	int		buffer_size;
	int		rest_size;

	buffer_size = ft_strlen(buffer);
	rest_size = ft_strlen(ft_strchr(buffer, '\n'));
	line = (char *) malloc(buffer_size - rest_size + 2);
	ft_strlcpy(line, buffer, buffer_size - rest_size + 1);
	return (line);
}

static char	*gnl_reset_buffer(char *buffer, char *line)
{
	char	*n_buffer;
	int		buffer_size;
	int		line_size;

	buffer_size = ft_strlen(buffer);
	line_size = ft_strlen(line);
	n_buffer = malloc(buffer_size - line_size + 2);
	ft_strlcpy(n_buffer, buffer + line_size + 1, buffer_size - line_size + 1);
	free(buffer);
	return (n_buffer);
}

static char	*gnl_extend_buffer(char *buffer, char *bytes)
{
	char	*n_buffer;
	int		buffer_size;
	int		bytes_size;

	buffer_size = ft_strlen(buffer);
	bytes_size = ft_strlen(bytes);
	n_buffer = (char *) malloc(buffer_size + bytes_size + 1);
	ft_strlcpy(n_buffer, buffer, buffer_size);
	ft_strlcat(n_buffer, bytes, buffer_size + bytes_size);
	free(buffer);
	return (n_buffer);
}

char	*get_next_line(int fd)
{
	char		bytes[BUFFER_SIZE + 1];
	static char	*buffer;
	char		*line;
	int			newline;

	if (!buffer)
		buffer = ft_strdup("");
	newline = 0;
	while (newline == 0 && read(fd, bytes, BUFFER_SIZE) > 0)
	{
		bytes[BUFFER_SIZE] = 0;
		if (ft_strchr(bytes, '\n'))
			newline = 1;
		buffer = gnl_extend_buffer(buffer, bytes);
	}
	line = gnl_get_line(buffer);
	buffer = gnl_reset_buffer(buffer, line);
	return (line);
}

