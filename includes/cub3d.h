/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 20:36:20 by tmurakam          #+#    #+#             */
/*   Updated: 2020/10/31 11:20:26 by tmurakam         ###   ########.fr       */
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
# define F_CUB_NOT_FOUND 0b1;
# define F_CUB_EXTNAME_E 0b01;

typedef struct	s_god
{
	void    *mlx;
    void    *win;
	void	*win_img;
	void	*img_addr;
	int img_bits_per_pixel;
	int	img_line_length;
	int img_endian;
	int wnd_x;
	int wnd_y;
    int scr_x;
    int scr_y;
	char *title;
	char *eo_fpath;
	char *wo_fpath;
	char *so_fpath;
	char *no_fpath;
	char *sp_fpath;
	int	ce_rgb;
	int	fl_rgb;
	char **map;
	int map_h;
	int map_w;
	int bmp;
	char *err_msg;		
}				t_god;	
#endif