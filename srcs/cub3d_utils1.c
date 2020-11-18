/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 11:45:27 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/19 02:10:57 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void			my_mlx_pixel_put(t_god *g, int x, int y, int color)
{
	char	*dst;

	dst = g->w_img.addr + (y * g->w_img.llen + x * (g->w_img.bpp / 8));
	*(unsigned int*)dst = color;
}

unsigned int	pic_color(t_img *img, int i, int j)
{
	if (0 <= i && i < img->x_size && 0 <= j && j <= img->y_size)
		return (*((unsigned int *)(img->addr + j * img->llen) + i));
	else
		return (0);
}

int				set_err_msg(t_god *g, char *msg)
{
	char *free_tmp;

	g->exit = 1;
	if (!g->err_msg)
		g->err_msg = ft_strdup("Error\n");
	if (g->err_msg)
	{
		free_tmp = g->err_msg;
		g->err_msg = ft_strjoin(g->err_msg, msg);
		free(free_tmp);
	}
	if (!g->err_msg)
	{
		write(2, "Error\nmemory error!!!\n", 22);
		exit(0);
	}
	return (0);
}

void			init_god(t_god *g)
{
	ft_bzero(g, sizeof(t_god));
	g->ce_rgb = -1;
	g->fl_rgb = -1;
	set_ivec(&g->scell, -1, -1);
	g->mlx = mlx_init();
	if (!g->mlx)
		set_err_msg(g, "Please check your XWINDOW settings\n");
	else
		mlx_get_screen_size(g->mlx, &g->scr_x, &g->scr_y);
}

void			destroy_god(t_god *g)
{
	if (g->s_img.p)
		mlx_destroy_image(g->mlx, g->s_img.p);
	if (g->ea_img.p)
		mlx_destroy_image(g->mlx, g->ea_img.p);
	if (g->we_img.p)
		mlx_destroy_image(g->mlx, g->we_img.p);
	if (g->so_img.p)
		mlx_destroy_image(g->mlx, g->so_img.p);
	if (g->no_img.p)
		mlx_destroy_image(g->mlx, g->no_img.p);
	if (g->w_img.p)
		mlx_destroy_image(g->mlx, g->w_img.p);
	if (g->win)
		mlx_destroy_window(g->mlx, g->win);
	if (g->map_list)
		ft_lstclear(&g->map_list, &free);
	while (g->map_h + 1 && g->map)
		free(*(g->map + g->map_h--));
	free(g->map);
	free(g->mlx);
	free(g->err_msg);
	free(g->cub_fname);
	free(g->bmp_fname);
	ft_bzero(g, sizeof(t_god));
}
