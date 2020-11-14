/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 20:36:20 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/14 17:22:48 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <math.h>
# include "../minilibx-linux/mlx.h"
# include "libft.h"
# include "get_next_line.h"

# define ABS(x) (0 < (x)) ? (x) : -(x)
# define MIN(a, b) ((a) < (b)) ? (a) : (b)
# define MAX(a, b) ((a) < (b)) ? (b) : (a)

# define MOVESPEED 0.01
# define ROTSPEED  0.01
# define WALLDIST  0.2

# define KEY_CODE_W     119
# define KEY_CODE_A      97
# define KEY_CODE_S     115
# define KEY_CODE_D     100
# define KEY_CODE_LA  65361
# define KEY_CODE_RA  65363
# define KEY_CODE_DA  65364
# define KEY_CODE_UA  65362
# define KEY_CODE_ESC 65307

# define X_EVENT_KEY_PRESS    2
# define X_EVENT_KEY_RELEASE  3
# define X_EVENT_KEY_EXIT    17

# define BMP_HEADER_SIZE 14
# define DIB_HEADER_SIZE 40
# define BMP_BPP 4
# define BMP_NUM_PLANES 1

# define CUB_COLOR_DUP "The cub file has double setting in ceiling or floor color.\n"

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

typedef struct	s_ivec
{
	int i;
	int j;
}				t_ivec;

typedef struct  s_dvec
{
	double x;
	double y;
}				t_fvec;

typedef struct	s_god
{
	char		*cub_fname;
	char		*bmp_fname;
	void		*mlx;
    void		*win;
	t_img		w_img;
	t_ivec		wnd;
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
	t_fvec		ppos;
	t_fvec		pdir;
	t_fvec		pvew;
	double		pdx;
	double		pdy;
	double		planex;
	double  	planey;
	double		plv;
	int			exit;
	double 		rotSpeed;
	double		moveSpeed_ga;
	double		moveSpeed_sw;
}				t_god;	

void	make_image(t_god *g);
void	paint_bg(t_god *g);
int		set_err_msg(t_god *g, char *msg);
void	set_ivec(t_ivec *v, long i, long j);
void	write_imgf(t_god *g);

#endif