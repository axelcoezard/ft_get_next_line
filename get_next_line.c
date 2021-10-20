/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 21:29:06 by acoezard          #+#    #+#             */
/*   Updated: 2021/10/20 16:42:36 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*gnl_get_line(char *buffer)
{
	char	*line;
	int		buffer_size;
	int		rest_size;

	if (!buffer)
		return (NULL);
	buffer_size = ft_strlen(buffer);
	rest_size = ft_strlen(ft_strchr(buffer, '\n')) - 1;
	if (rest_size > buffer_size)
		return (NULL);
	line = (char *) malloc(buffer_size - rest_size + 1);
	ft_strlcpy(line, buffer, buffer_size - rest_size + 1);
	return (line);
}

static char	*gnl_reset_buffer(char *buffer, char *line)
{
	char	*n_buffer;
	int		buffer_size;
	int		line_size;

	if (!buffer || !line)
		return (NULL);
	buffer_size = ft_strlen(buffer);
	line_size = ft_strlen(line);
	n_buffer = malloc(buffer_size - line_size + 1);
	ft_strlcpy(n_buffer, buffer + line_size, buffer_size - line_size);
	free(buffer);
	return (n_buffer);
}

static char	*gnl_extend_buffer(char *buffer, char *bytes)
{
	char	*n_buffer;
	int		buffer_size;
	int		bytes_size;

	if (!buffer || !bytes)
		return (NULL);
	buffer_size = ft_strlen(buffer);
	bytes_size = ft_strlen(bytes);
	n_buffer = (char *) malloc(buffer_size + bytes_size + 1);
	ft_strlcpy(n_buffer, buffer, buffer_size + 1);
	ft_strlcat(n_buffer, bytes, buffer_size + bytes_size + 1);
	free(buffer);
	return (n_buffer);
}

static char	*gnl_get_until_newline(int fd, char *buffer)
{
	char	*bytes;
	int		reader;

	reader = 1;
	bytes = (char *) malloc(BUFFER_SIZE + 1);
	while (!ft_strchr(buffer, '\n') && reader != 0)
	{
		reader = read(fd, bytes, BUFFER_SIZE);
		if (reader == -1)
		{
			free(buffer);
			free(bytes);
			return (NULL);
		}
		bytes[BUFFER_SIZE] = 0;
		buffer = gnl_extend_buffer(buffer, bytes);
	}
	free(bytes);
	if (reader == 0)
	{
		free(buffer);
		return (NULL);
	}
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!buffer)
		buffer = ft_strdup("");
	buffer = gnl_get_until_newline(fd, buffer);
	if (!buffer)
		return (NULL);
	line = gnl_get_line(buffer);
	buffer = gnl_reset_buffer(buffer, line);
	return (line);
}
