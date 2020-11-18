/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 06:15:14 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/18 20:54:56 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	loop_func(t_god *g)
{
	rotation(&g->pdir, g->rotspeed);
	rotation(&g->pvew, g->rotspeed);
	next_pl(g);
	paint_bg(g);
	make_image(g);
	if (g->bmp)
		write_imgf(g);
	if (g->exit)
		exit_func(g);
	mlx_put_image_to_window(g->mlx, g->win, g->w_img.p, 0, 0);
	mlx_do_sync(g->mlx);
	return (0);
}

int	hook_keypress_func(int key_code, t_god *g)
{
	if (key_code == KEY_CODE_W)
		g->movespeed_ga += MOVESPEED;
	if (key_code == KEY_CODE_S)
		g->movespeed_ga -= MOVESPEED;
	if (key_code == KEY_CODE_D)
		g->movespeed_sw += MOVESPEED;
	if (key_code == KEY_CODE_A)
		g->movespeed_sw -= MOVESPEED;
	if (key_code == KEY_CODE_LA)
		g->rotspeed += ROTSPEED;
	if (key_code == KEY_CODE_RA)
		g->rotspeed -= ROTSPEED;
	if (key_code == KEY_CODE_UA)
		g->movespeed_ga += MOVESPEED;
	if (key_code == KEY_CODE_DA)
		g->movespeed_ga -= MOVESPEED;
	if (key_code == KEY_CODE_ESC)
		g->exit = 1;
	return (0);
}

int	hook_keyrelease_func(int key_code, t_god *g)
{
	if (key_code == KEY_CODE_W)
		g->movespeed_ga -= MOVESPEED;
	if (key_code == KEY_CODE_S)
		g->movespeed_ga += MOVESPEED;
	if (key_code == KEY_CODE_D)
		g->movespeed_sw -= MOVESPEED;
	if (key_code == KEY_CODE_A)
		g->movespeed_sw += MOVESPEED;
	if (key_code == KEY_CODE_LA)
		g->rotspeed -= ROTSPEED;
	if (key_code == KEY_CODE_RA)
		g->rotspeed += ROTSPEED;
	if (key_code == KEY_CODE_UA)
		g->movespeed_ga -= MOVESPEED;
	if (key_code == KEY_CODE_DA)
		g->movespeed_ga += MOVESPEED;
	return (0);
}

int	hook_exit_func(t_god *g)
{
	mlx_do_sync(g->mlx);
	g->exit = 1;
	exit_func(g);
	return (0);
}

int	main(int argc, char **argv)
{
	t_god	g;

	init_god(&g);
	if (!g.err_msg)
		load_settings(&g, argc, argv);
	if (g.err_msg)
	{
		write(1, g.err_msg, ft_strlen(g.err_msg));
		destroy_god(&g);
		return (0);
	}
	else
	{
		if (!g.bmp)
		{
			g.win = mlx_new_window(g.mlx, g.wnd.i, g.wnd.j, g.cub_fname);
			mlx_hook(g.win, X_E_P, X_M_P, &hook_keypress_func, &g);
			mlx_hook(g.win, X_E_R, X_M_R, &hook_keyrelease_func, &g);
			mlx_hook(g.win, X_E_E, X_M_E, &hook_exit_func, &g);
			mlx_loop_hook(g.mlx, &loop_func, &g);
			mlx_loop(g.mlx);
		}
		loop_func(&g);
		return (0);
	}
}
