/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 21:29:06 by acoezard          #+#    #+#             */
/*   Updated: 2021/10/20 00:43:07 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*gnl_get_line(char *buffer)
{

}

static void	gnl_reset_buffer(char *buffer, char *line)
{

}

static char	*gnl_extend_buffer(char *buffer, char *bytes)
{
	char	*n_buffer;
	int		i;
	int		j;

	n_buffer = (char *) malloc(ft_strlen(buffer) + ft_strlen(bytes) + 1);
	i = 0;
	while (buffer[i] != 0)
		n_buffer[i] = buffer[i++];
	j = 0;
	while (bytes[j] != 0)
		n_buffer[i + j] = buffer[j++];
	n_buffer[i + j] = 0;
	return (n_buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;
	char		*bytes;
	int			newline;

	newline = 0;
	if (buffer == NULL)
		buffer = ft_strdup("");
	bytes = (char *) malloc(BUFFER_SIZE + 1);
	while (newline == 0 && read(fd, bytes, BUFFER_SIZE) > 0)
	{
		buffer = gnl_extend_buffer(buffer, bytes);
		if (ft_strchr(bytes, '\n'))
			newline = 1;
	}
	line = gnl_get_line(buffer);
	gnl_reset_buffer(buffer, line);
	free(bytes);
	return (line);
}

int	main(void)
{
	int filedesc;

	filedesc = open("text.txt", O_RDONLY);
	if (filedesc != -1)
	{
		printf("%s", get_next_line(filedesc));
		get_next_line(filedesc);
		printf("%s", get_next_line(filedesc));
		printf("%s", get_next_line(filedesc));
		close(filedesc);
	}
}
