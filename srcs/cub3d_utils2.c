/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 11:52:54 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/15 11:53:27 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	exit_func(t_god *g)
{
	destroy_god(g);
	exit(0);
}

void	next_pl(t_god *g)
{
	t_fvec	step;
	double	wdist;

	step.x = g->pdir.x * g->moveSpeed_ga + g->pdir.y * g->moveSpeed_sw;
	step.y = g->pdir.y * g->moveSpeed_ga - g->pdir.x * g->moveSpeed_sw;
	wdist = 0 < step.x ? WALLDIST : -WALLDIST;
	if (g->map[(int)(g->ppos.x + step.x + wdist)][(int)(g->ppos.y)] == '1')
		step.x = 0;
	wdist = 0 < step.y ? WALLDIST : -WALLDIST;
	if (g->map[(int)(g->ppos.x)][(int)(g->ppos.y + step.y + wdist)] == '1')
		step.y = 0;
	set_fvec(&g->ppos, g->ppos.x + step.x, g->ppos.y + step.y);
}

void	paint_bg(t_god *g)
{
	t_ivec wnd;

	wnd.i = -1;
	while (++wnd.i < g->wnd.i)
	{
		wnd.j = -1;
		while (++wnd.j < g->wnd.j / 2)
			my_mlx_pixel_put(g, wnd.i, wnd.j, g->ce_rgb);
		while (++wnd.j < g->wnd.j)
			my_mlx_pixel_put(g, wnd.i, wnd.j, g->fl_rgb);
	}
}
