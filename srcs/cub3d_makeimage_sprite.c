/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_makeimage_sprite.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 11:26:47 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/24 11:54:51 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static double	f_invdet(t_god *g)
{
	return (1.0 / (g->pvew.x * g->pdir.y - g->pdir.x * g->pvew.y));
}

static t_fvec	f_transform(t_god *g, int i, int j)
{
	t_fvec	ret_fvec;
	t_fvec	spos;

	set_fvec(&spos, i + 0.5 - g->ppos.x, j + 0.5 - g->ppos.y);
	ret_fvec.x = f_invdet(g) * (g->pdir.y * spos.x - g->pdir.x * spos.y);
	ret_fvec.y = f_invdet(g) * (-g->pvew.y * spos.x + g->pvew.x * spos.y);
	return (ret_fvec);
}

static void		write_a_sprt_loop(t_god *g, t_ivec *de, int x, int *ymask)
{
	int				y;
	unsigned int	color;
	t_ivec			tex;

	tex.i = (int)(((double)x - (-g->i_s_size / 2 + g->i_s_scr_x)) *
		g->s_img.x_size / g->i_s_size);
	y = de->i;
	while (y <= de->j)
	{
		tex.j = (int)(((double)(y - g->wnd.j / 2 + g->i_s_size / 2) *
		g->s_img.y_size) / g->i_s_size);
		color = pic_color(&g->s_img, tex.i, tex.j);
		if (color != g->i_zero_color && ymask[y] == 0)
		{
			ymask[y] = 1;
			my_mlx_pixel_put(g, x, y, color);
		}
		y++;
	}
}

static void		write_a_sprt(t_god *g, int x, int *ymask, t_ivec *s_cell)
{
	t_fvec			transform;
	t_ivec			de;

	transform = f_transform(g, s_cell->i, s_cell->j);
	g->i_s_scr_x = (int)((g->wnd.i / 2) * (1 + transform.x / transform.y));
	g->i_s_size = ABS((int)(g->wnd.j / (transform.y)));
	de.i = MAX(0, -g->i_s_size / 2 + g->wnd.j / 2);
	de.j = MIN(g->i_s_size / 2 + g->wnd.j / 2, g->wnd.j - 1);
	if (transform.y <= 0)
		return ;
	if (x < -g->i_s_size / 2 + g->i_s_scr_x ||
		g->i_s_size / 2 + g->i_s_scr_x <= x)
		return ;
	write_a_sprt_loop(g, &de, x, ymask);
}

void			sprt_verline(t_god *g, int x, int *mx)
{
	t_ivec			s_cell;
	int				ymask[g->wnd.j];

	ft_bzero(ymask, sizeof(int) * g->wnd.j);
	g->i_zero_color = pic_color(&g->s_img, 0, 0);
	while (*mx)
	{
		set_ivec(&s_cell, *mx, *(mx + 1));
		write_a_sprt(g, x, ymask, &s_cell);
		mx += 2;
	}
}
