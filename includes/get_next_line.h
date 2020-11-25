/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/01 22:22:02 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/25 22:36:53 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <limits.h>
# include "libft.h"
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 256
# endif

int		get_next_line(int fd, char **line);
void	*ft_salloc(size_t count, size_t size);
int		free_and_ret(int rc, char **line, char **buf, char **tmp_str);
int		line_feed(char **line, char **buf, char **tmp_str);

#endif
