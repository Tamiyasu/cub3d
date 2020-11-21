/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bmpwrite.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 10:31:00 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/21 19:21:20 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	littleendian(unsigned char *s, unsigned int n, int size)
{
	int i;

	i = 0;
	s[i++] = (unsigned char)(n);
	while (i < size)
		s[i++] = (unsigned char)(n >>= 8);
}

static void	write_header(int fd, t_god *g)
{
	unsigned char	header[BMP_HEADER_SIZE + DIB_HEADER_SIZE];

	ft_bzero(header, BMP_HEADER_SIZE + DIB_HEADER_SIZE);
	ft_memcpy(header, "BM", 2);
	littleendian(header + 2, BMP_HEADER_SIZE + DIB_HEADER_SIZE +
		(BMP_BPP * g->wnd.i * g->wnd.j), 4);
	littleendian(header + 10, BMP_HEADER_SIZE + DIB_HEADER_SIZE, 4);
	littleendian(header + 14, DIB_HEADER_SIZE, 4);
	littleendian(header + 18, g->wnd.i, 4);
	littleendian(header + 22, g->wnd.j, 4);
	littleendian(header + 26, BMP_NUM_PLANES, 2);
	littleendian(header + 28, BMP_BPP * 8, 2);
	write(fd, header, BMP_HEADER_SIZE + DIB_HEADER_SIZE);
}

static int	write_pixels(int fd, t_god *g)
{
	t_ivec			it;
	unsigned char	*line;

	if (!(line = ft_calloc(sizeof(char), g->wnd.i * BMP_BPP)))
		return (-1);
	it.i = g->wnd.j;
	while (--it.i >= 0)
	{
		it.j = -1;
		while (++it.j < g->wnd.i)
			littleendian(line + (it.j * BMP_BPP),
			*((unsigned int *)(g->w_img.addr +
			it.i * g->w_img.llen) + it.j), 4);
		write(fd, line, g->wnd.i * BMP_BPP);
	}
	free(line);
	return (0);
}

void		write_imgf(t_god *g)
{
	int		fd;

	g->exit = 1;
	if (!(g->bmp_fname = ft_strdup(g->cub_fname)))
	{
		set_err_msg(g, "memory alocate error in making bmp file.");
		return ;
	}
	ft_memcpy(g->bmp_fname + ft_strlen(g->bmp_fname) - 3, "bmp", 3);
	if ((fd = open(g->bmp_fname, O_CREAT | O_WRONLY, 0666)) == -1)
	{
		set_err_msg(g, "file open error in making bmp file.");
		return ;
	}
	write_header(fd, g);
	if (write_pixels(fd, g) == -1)
		set_err_msg(g, "Failed to open bmp file\n");
	close(fd);
	return ;
}
