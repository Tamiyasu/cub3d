/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_initialize_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 11:15:54 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/15 12:15:50 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static t_img	*set_target(t_god *g, char *str, int *start_pos)
{
	*start_pos = 2;
	if (!ft_memcmp(str, "S ", 2))
	{
		*start_pos = 1;
		return (&g->s_img);
	}
	if (!ft_memcmp(str, "SO ", 3))
		return (&g->so_img);
	if (!ft_memcmp(str, "WE ", 3))
		return (&g->we_img);
	if (!ft_memcmp(str, "NO ", 3))
		return (&g->no_img);
	if (!ft_memcmp(str, "EA ", 3))
		return (&g->ea_img);
	return (NULL);
}

void			read_nmb(char **str, int *int_p, int max, int max_error)
{
	skip_spacis(str);
	if (ft_isdigit(**str))
		*int_p = 0;
	while (ft_isdigit(**str))
	{
		*int_p = *int_p * 10 + *(*str)++ - '0';
		if (max < *int_p)
			*int_p = max_error ? max_error : max;
	}
	skip_spacis(str);
}

void			read_r(t_god *g, char *str)
{
	if (g->wnd.i || g->wnd.j)
	{
		set_err_msg(g, "The cub file has double setting in window size.\n");
		return ;
	}
	str++;
	read_nmb(&str, &g->wnd.i, g->scr_x, 0);
	read_nmb(&str, &g->wnd.j, g->scr_y, 0);
	if (*str)
		set_ivec(&g->wnd, 0, 0);
	if (!g->wnd.i || !g->wnd.j)
	{
		set_err_msg(g, "The cub file has wrong setting in window size.\n");
		return ;
	}
}

void			read_color(t_god *g, char *str)
{
	int *target_p;
	int red;
	int green;
	int blue;

	red = 256;
	green = 256;
	blue = 256;
	target_p = *str == 'C' ? &g->ce_rgb : &g->fl_rgb;
	str++;
	read_nmb(&str, &red, g->scr_y, 256);
	str += *str == ',' ? 1 : 0;
	read_nmb(&str, &green, g->scr_y, 256);
	str += *str == ',' ? 1 : 0;
	read_nmb(&str, &blue, g->scr_y, 256);
	if (*target_p != -1)
		set_err_msg(g, "Don't set ceiling or floor color twice\n");
	if (255 < red || 255 < green || 255 < blue || *str)
		set_err_msg(g, "Inaccseptable setting in ceiling or floor color.\n");
	*target_p = (red << 16 | green << 8 | blue);
}

void			read_img(t_god *g, char *str)
{
	char	*f_name;
	t_img	*img;
	int		start_pos;

	img = set_target(g, str, &start_pos);
	str += start_pos;
	f_name = ft_strtrim(str, " \t\n\v\f\r");
	if (img->p)
	{
		set_err_msg(g, "Don't set texture twice.\n");
		free(f_name);
		return ;
	}
	img->p = mlx_xpm_file_to_image(g->mlx, f_name, &img->x_size, &img->y_size);
	if (!img->p)
		set_err_msg(g, "A texture file is not able to load.\n");
	else
		img->addr = (int *)mlx_get_data_addr(img->p,
			&img->bpp, &img->llen, &img->endian);
	free(f_name);
}
