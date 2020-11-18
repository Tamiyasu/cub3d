/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_makeimage.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 11:21:44 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/19 03:08:44 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	set_mapi(t_ivec *mapi, t_fvec *sidedist, t_ivec *step)
{
	if (sidedist->x < sidedist->y)
		mapi->i += step->i;
	else
		mapi->j += step->j;
}

static int	find_w_n_s(t_god *g, t_fvec *ray_dir, t_ivec *mapi, int *mx)
{
	int		mxi;
	int		side;
	t_fvec	sidedist;
	t_ivec	step;

	sidedist = f_sidedist(g, mapi, ray_dir);
	set_ivec(&step, ray_dir->x < 0 ? -1 : 1, ray_dir->y < 0 ? -1 : 1);
	mxi = 0;
	while (g->map[mapi->i][mapi->j] != '1')
	{
		if (g->map[mapi->i][mapi->j] == '2')
		{
			mx[mxi * 2] = mapi->i;
			mx[mxi * 2 + 1] = mapi->j;
			mxi++;
		}
		side = sidedist.x < sidedist.y ? 0 : 1;
		set_mapi(mapi, &sidedist, &step);
		sidedist = f_sidedist(g, mapi, ray_dir);
	}
	return (side);
}

static void	wall_verline(t_god *g, int x)
{
	t_ivec			de;
	t_ivec			tp;
	int				y;
	unsigned int	color;

	de.i = -(int)(g->wnd.j / g->i_perpdist) / 2 + g->wnd.j / 2;
	de.j = (int)(g->wnd.j / g->i_perpdist) / 2 + g->wnd.j / 2;
	tp.i = (int)(g->i_tx * (double)(g->i_img->x_size));
	y = de.i;
	while (y <= de.j)
	{
		tp.j = (int)((double)(y - de.i) / (de.j - de.i) * g->i_img->y_size);
		color = pic_color(g->i_img, tp.i, tp.j);
		if (0 <= y && y < g->wnd.j)
			my_mlx_pixel_put(g, x, y, color);
		y++;
	}
}

static int	write_vertical_line(t_god *g, int x)
{
	int		*mx;

	if (!(mx = ft_calloc(sizeof(int), MAX(g->map_h, g->map_w) * 4)))
		return (set_err_msg(g, "malloc error!"));
	g->i_ray_dir = f_ray_dir(g, x);
	set_ivec(&g->i_mapi, (int)(g->ppos.x), (int)(g->ppos.y));
	g->i_side = find_w_n_s(g, &g->i_ray_dir, &g->i_mapi, mx);
	g->i_perpdist = f_perpdist(g, &g->i_mapi, &g->i_ray_dir, g->i_side);
	g->i_tx = f_tx(g, g->i_perpdist, &g->i_ray_dir, g->i_side);
	g->i_img = f_texture_im(g);
	wall_verline(g, x);
	sprt_verline(g, x, mx);
	free(mx);
	return (0);
}

void		make_image(t_god *g)
{
	int x;

	x = 0;
	while (x < g->wnd.i)
		write_vertical_line(g, x++);
}
