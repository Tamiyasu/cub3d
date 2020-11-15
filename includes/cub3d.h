/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 20:36:20 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/15 11:55:49 by tmurakam         ###   ########.fr       */
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
	char			*cub_fname;
	char			*bmp_fname;
	void			*mlx;
    void			*win;
	t_img			w_img;
	t_ivec			wnd;
    int				scr_x;
    int				scr_y;
	char			*title;
	t_img			ea_img;
	t_img			we_img;
	t_img			so_img;
	t_img			no_img;
	t_img			s_img;
	int				ce_rgb;
	int				fl_rgb;
	char			**map;
	size_t			map_h;
	size_t			map_w;
	int				bmp;
	char			*err_msg;		
	t_list			*map_list;
	t_fvec			ppos;
	t_fvec			pdir;
	t_fvec			pvew;
	int				exit;
	double 			rotSpeed;
	double			moveSpeed_ga;
	double			moveSpeed_sw;
	t_fvec			i_ray_dir;
	t_ivec			i_mapi;
	int				i_side;
	double			i_perpdist;
	double			i_tx;
	t_img			*i_img;
	unsigned int	i_zero_color;
	int				i_s_scr_x;
	int				i_s_size;
}				t_god;

void			exit_func(t_god *g);
void			next_pl(t_god *g);
void			paint_bg(t_god *g);
int				set_err_msg(t_god *g, char *msg);
unsigned int	pic_color(t_img *img, int i, int j);
void			destroy_god(t_god *g);
void			init_god(t_god *g);
void			my_mlx_pixel_put(t_god *g, int x, int y, int color);
void			ft_lstdelhead(t_list **lst, void (*del)(void *));
void			rotation(t_fvec *v, double rot);
void			set_ivec(t_ivec *v, long i, long j);
void			set_fvec(t_fvec *v, double x, double y);
void			skip_spacis(char **str);
void			read_color(t_god *g, char *str);
void			read_img(t_god *g, char *str);
void			read_nmb(char **str, int *int_p, int max, int max_error);
void			read_r(t_god *g, char *str);
double			f_perpdist(t_god *g, t_ivec *mapi, t_fvec *ray_dir, int side);
double			f_tx(t_god *g, double perpdist, t_fvec *ray_dir, int side);
t_fvec			f_ray_dir(t_god *g, int x);
t_fvec			f_sidedist(t_god *g, t_ivec *mapi, t_fvec *ray_dir);
t_img			*f_texture_im(t_god *g);
void			sprt_verline(t_god *g, int x, int *mx);
void			make_image(t_god *g);
int 			load_settings(t_god *g, int argc, char **argv);
void			eval_map(t_god *g, char *line, char *cfg_str, char *no_map_str);
int				map_closecheck(t_ivec start_pos, char **map, t_god *g);
void			write_imgf(t_god *g);

#endif