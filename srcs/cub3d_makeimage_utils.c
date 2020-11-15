/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_makeimage_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 11:36:35 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/15 12:05:34 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_fvec	f_sidedist(t_god *g, t_ivec *mapi, t_fvec *ray_dir)
{
	t_fvec delta_dist;
	t_fvec ret_fvec;

	set_fvec(&delta_dist, ABS(1 / ray_dir->x), ABS(1 / ray_dir->y));
	if (ray_dir->x < 0)
		ret_fvec.x = (g->ppos.x - mapi->i) * delta_dist.x;
	else
		ret_fvec.x = (mapi->i + 1.0 - g->ppos.x) * delta_dist.x;
	if (ray_dir->y < 0)
		ret_fvec.y = (g->ppos.y - mapi->j) * delta_dist.y;
	else
		ret_fvec.y = (mapi->j + 1.0 - g->ppos.y) * delta_dist.y;
	return (ret_fvec);
}

t_fvec	f_ray_dir(t_god *g, int x)
{
	t_fvec ret_fvec;

	ret_fvec.x = g->pdir.x + g->pvew.x * (2 * x / (double)(g->wnd.i) - 1);
	ret_fvec.y = g->pdir.y + g->pvew.y * (2 * x / (double)(g->wnd.i) - 1);
	return (ret_fvec);
}

double	f_perpdist(t_god *g, t_ivec *mapi, t_fvec *ray_dir, int side)
{
	t_ivec step;

	set_ivec(&step, ray_dir->x < 0 ? -1 : 1, ray_dir->y < 0 ? -1 : 1);
	return (side == 0 ?
		(mapi->i - g->ppos.x + (1 - step.i) / 2) / ray_dir->x :
		(mapi->j - g->ppos.y + (1 - step.j) / 2) / ray_dir->y);
}

double	f_tx(t_god *g, double perpdist, t_fvec *ray_dir, int side)
{
	double ret_f;

	ret_f = side == 0 ? g->ppos.y + perpdist * ray_dir->y :
						g->ppos.x + perpdist * ray_dir->x;
	ret_f -= floor(ret_f);
	if ((side == 0 && ray_dir->x > 0) || (side == 1 && ray_dir->y < 0))
		ret_f = 1 - ret_f;
	return (ret_f);
}

t_img	*f_texture_im(t_god *g)
{
	if (g->i_side == 0 && g->i_ray_dir.x < 0)
		return (&g->no_img);
	if (g->i_side == 0 && g->i_ray_dir.x > 0)
		return (&g->so_img);
	if (g->i_side == 1 && g->i_ray_dir.y > 0)
		return (&g->ea_img);
	if (g->i_side == 1 && g->i_ray_dir.y < 0)
		return (&g->we_img);
	return (NULL);
}
