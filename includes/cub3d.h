/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 20:36:20 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/07 20:15:52 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
#include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "../minilibx-linux/mlx.h"
# include "libft.h"
# include "get_next_line.h"

# define ABS(x) (0 < (x)) ? (x) : -(x)

typedef struct	s_img
{
	void	*p;
	void	*addr;
	int		bpp;
	int		llen;
	int		endian;
	int		x_size;
	int		y_size;
}				t_img;

typedef struct	s_pos
{
	int i;
	int j;
}				t_pos;


typedef struct	s_god
{
	void		*mlx;
    void		*win;
	t_img		w_img;
	int			wnd_x;
	int			wnd_y;
    int			scr_x;
    int			scr_y;
	char		*title;
	t_img		ea_img;
	t_img		we_img;
	t_img		so_img;
	t_img		no_img;
	t_img		s_img;
	int			ce_rgb;
	int			fl_rgb;
	char		**map;
	int			map_h;
	int			map_w;
	int			bmp;
	char		*err_msg;		
	t_list		*map_list;
	double		pli;
	double  	plj;
	double		pdx;
	double  	pdy;
	double		planex;
	double  	planey;
	double		plv;
	int			exit;
}				t_god;	

void set_err_msg(t_god *g, char *msg);
void print_game(t_god *g); //please remove before finish;
#endif