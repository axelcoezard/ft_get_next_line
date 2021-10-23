/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 21:29:06 by acoezard          #+#    #+#             */
/*   Updated: 2021/10/23 13:16:50 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*gnl_get_line(int fd, char *backup[MAX_FD])
{
	char	*line;
	int		backup_size;
	int		rest_size;

	if (backup[fd] == NULL)
		return (NULL);
	backup_size = ft_strlen(backup[fd]);
	rest_size = ft_strlen(ft_strchr(backup[fd], '\n')) - 1;
	line = (char *) malloc(backup_size - rest_size + 1);
	ft_strlcpy(line, backup[fd], backup_size - rest_size + 1);
	return (line);
}

static char	*gnl_reset_backup(int fd, char *backup[MAX_FD], char *line)
{
	char	*n_backup;
	int		backup_size;
	int		line_size;

	if (backup[fd] == NULL || line == NULL)
		return (NULL);
	backup_size = ft_strlen(backup[fd]);
	line_size = ft_strlen(line);
	if (line_size <= 0)
	{
		free(backup[fd]);
		return (NULL);
	}
	n_backup = malloc(backup_size - line_size + 1);
	ft_strlcpy(n_backup, backup[fd] + line_size, backup_size - line_size + 1);
	free(backup[fd]);
	return (n_backup);
}

static char	*gnl_extend_backup(int fd, char *backup[MAX_FD], char *bytes)
{
	char	*n_backup;
	int		backup_size;
	int		bytes_size;

	if (backup[fd] == NULL || bytes == NULL)
		return (NULL);
	backup_size = ft_strlen(backup[fd]);
	bytes_size = ft_strlen(bytes);
	n_backup = (char *) malloc(backup_size + bytes_size + 1);
	ft_strlcpy(n_backup, backup[fd], backup_size + 1);
	ft_strcat(n_backup, bytes);
	free(backup[fd]);
	return (n_backup);
}

static char	*gnl_get_until_newline(int fd, char *backup[MAX_FD])
{
	char	*bytes;
	int		reader;

	bytes = (char *) malloc(BUFFER_SIZE + 1);
	if (!bytes || read(fd, bytes, 0) < 0)
	{
		free(bytes);
		free(backup[fd]);
		return (NULL);
	}
	reader = 1;
	while (!ft_strchr(backup[fd], '\n') && reader != 0)
	{
		reader = read(fd, bytes, BUFFER_SIZE);
		if (reader == -1)
		{
			free(backup[fd]);
			free(bytes);
			return (NULL);
		}
		bytes[reader] = 0;
		backup[fd] = gnl_extend_backup(fd, backup, bytes);
	}
	free(bytes);
	return (backup[fd]);
}

char	*get_next_line(int fd)
{
	static char	*backup[MAX_FD];
	char		*line;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	if (backup[fd] == NULL)
		backup[fd] = ft_strdup("");
	backup[fd] = gnl_get_until_newline(fd, backup);
	if (backup[fd] == NULL)
		return (NULL);
	line = gnl_get_line(fd, backup);
	backup[fd] = gnl_reset_backup(fd, backup, line);
	if (line[0] == '\0')
	{
		free(backup[fd]);
		free(line);
		return (NULL);
	}
	return (line);
}
