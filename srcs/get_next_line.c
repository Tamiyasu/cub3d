/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/03 19:40:42 by tmurakam          #+#    #+#             */
/*   Updated: 2020/10/30 18:15:52 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		get_next_line(int fd, char **line)
{
	static char	*buf = NULL;
	ssize_t		read_len;
	char		*tmp_str;

	*line = ft_salloc(1, sizeof(char));
	if (!buf)
		buf = ft_salloc(BUFFER_SIZE + 1, sizeof(char));
	while (buf)
	{
		if (ft_strlen(buf))
		{
			if (1 == line_feed(line, &buf, &tmp_str))
				return (1);
		}
		read_len = read(fd, buf, BUFFER_SIZE);
		if (read_len <= 0)
			return (free_and_ret(-!!read_len, NULL, &buf, NULL));
		*(buf + read_len) = '\0';
	}
	return (-1);
}

int		line_feed(char **line, char **buf, char **tmp_str)
{
	if (ft_strchr(*buf, '\n'))
	{
		*tmp_str = *line;
		if (!(*line = ft_salloc(ft_strlen(*line) +
		(ft_strchr(*buf, '\n') - *buf) + 1, sizeof(char))))
			return (free_and_ret(-1, line, buf, tmp_str));
		ft_strlcpy(*line, *tmp_str, ft_strlen(*tmp_str) + 1);
		ft_strlcat(*line, *buf,
			ft_strlen(*line) + ft_strchr(*buf, '\n') - *buf + 1);
		ft_strlcpy(*buf, ft_strchr(*buf, '\n') + 1,
		BUFFER_SIZE - (ft_strchr(*buf, '\n') - *buf));
		return (free_and_ret(1, NULL, NULL, tmp_str));
	}
	else
	{
		*tmp_str = *line;
		if (!(*line = ft_strjoin(*line, *buf)))
			return (free_and_ret(-1, line, buf, tmp_str));
		return (free_and_ret(INT_MIN, NULL, NULL, tmp_str));
	}
}

int		free_and_ret(int rc, char **line, char **buf, char **tmp_str)
{
	if (line)
	{
		free(*line);
		*line = NULL;
	}
	if (buf)
	{
		free(*buf);
		*buf = NULL;
	}
	if (tmp_str)
	{
		free(*tmp_str);
		*tmp_str = NULL;
	}
	return (rc);
}

void	*ft_salloc(size_t count, size_t size)
{
	char	*return_p;

	if (!(return_p = malloc(count * size)))
		return (return_p);
	*return_p = '\0';
	*(return_p + count * size - 1) = '\0';
	return ((void *)return_p);
}
