/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 06:15:14 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/14 10:51:54 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	rotation(t_fvec *v, double rot)
{
	double tmpx;

	tmpx = v->x;
	v->x = v->x * cos(rot) - v->y * sin(rot);
	v->y = tmpx * sin(rot) + v->y * cos(rot);
}

void	init_god(t_god *g)
{
	ft_bzero(g, sizeof(t_god));
	g->ce_rgb = -1;
	g->fl_rgb = -1;
	g->mlx = mlx_init();
	if(!g->mlx)
		set_err_msg(g, "Please check your XWINDOW settings\n");
	else
		mlx_get_screen_size(g->mlx, &g->scr_x, &g->scr_y);
}

void	my_mlx_pixel_put(t_god *g, int x, int y, int color)
{
	char	*dst;

	dst = g->w_img.addr + (y * g->w_img.llen + x * (g->w_img.bpp / 8));
	*(unsigned int*)dst = color;
}

void skip_spacis(char **str)
{
	while(ft_isspace(**str))
	(*str)++;
}

/*read digits string and set the number in *int_p  *
*set the max       in *int_p if max_error is     0*
*set the max_error in *int_p if max_error is NOT 0*
*the *str pointer proceeds first char that is not digit and spacis*/
void read_nmb(char **str, int *int_p, int max, int max_error)
{
	skip_spacis(str);
	if(ft_isdigit(**str))
		*int_p = 0;
	while(ft_isdigit(**str))
	{
		*int_p = *int_p * 10 + *(*str)++ - '0';
		if (max < *int_p)
			*int_p = max_error ? max_error : max;
	}
	skip_spacis(str);
}

void read_r(t_god *g, char *str, int line_count)
{
	if (g->wnd.i || g->wnd.j)
	{
		set_err_msg(g, "The cub file has double setting in window size.\n");
		return;
	}
	str++;
	read_nmb(&str, &g->wnd.i, g->scr_x, 0);
	read_nmb(&str, &g->wnd.j, g->scr_y, 0);
	if (*str)
		set_ivec(&g->wnd, 0, 0);
	if (!g->wnd.i || !g->wnd.j)
	{
		set_err_msg(g, "The cub file has wrong setting in window size.\n");
		return;
	}
}

void set_ivec(t_ivec *v, long i, long j)
{
	v->i = i;
	v->j = j;
}

void set_fvec(t_fvec *v, double x, double y)
{
	v->x = x;
	v->y = y;
}

void set_r_on_map(char **map, int i, int j, t_list **lst_last)
{
	t_ivec *tmp;

	if (map[i][j] != '1' && map[i][j] != 'R')
	{
		map[i][j] = 'R';
		tmp = malloc(sizeof(t_ivec));
		set_ivec(tmp, i, j);
		(*lst_last)->next = ft_lstnew(tmp);
		*lst_last = (*lst_last)->next;
	}
}

void set_r_around(char **map, t_ivec *v, t_list **lst_last)
{
	set_r_on_map(map, v->i, v->j - 1, lst_last);
	set_r_on_map(map, v->i, v->j + 1, lst_last);
	set_r_on_map(map, v->i - 1, v->j, lst_last);
	set_r_on_map(map, v->i + 1, v->j, lst_last);
}

void ft_lstdelhead(t_list **lst, void (*del)(void *))
{
	t_list *lst_tmp;

	lst_tmp = *lst;
	*lst = (*lst)->next;
	ft_lstdelone(lst_tmp, del);
}

int check_around(t_god *g, char **map, t_ivec *v)
{
	return(v->i == 0 || v->i == g->map_h - 1 ||
		v->j == 0 || v->j == g->map_w - 1 ||
		map[v->i][v->j - 1] == ' ' || map[v->i - 1][v->j] == ' ' ||
		map[v->i][v->j + 1] == ' ' || map[v->i + 1][v->j] == ' ');
}

int map_closecheck(t_ivec start_pos, char **map, t_god *g)
{
	t_list *lst;
	t_list *lst_last;
	t_ivec *pos;

	lst = NULL;
	lst_last = NULL;
	if (!(pos = malloc(sizeof(t_ivec))))
		return (set_err_msg(g, "malloc error!"));
	set_ivec(pos, start_pos.i, start_pos.j);
	map[pos->i][pos->j] = 'R';

	lst = ft_lstnew(pos);
	lst_last = lst;
	while(lst)
	{
		if (check_around(g, map, (t_ivec *)(lst->content)))
		{
			ft_lstclear(&lst, &free);
			return (set_err_msg(g, "map is not closed.\n"));
		}
		set_r_around(map, (t_ivec *)(lst->content), &lst_last);
		ft_lstdelhead(&lst, &free);
	}
	return (0);
}

void free_2d(void **v, size_t i_size)
{
	while(i_size--)
		free(*(v + i_size));
	free(v);
}

void set_start_pos(t_god *g, t_ivec *p_pos)
{
	t_fvec player_pos;
	t_fvec player_direction;
	t_fvec player_view_plain;
	double rot;

	set_fvec(&player_pos, p_pos->j + 0.5, p_pos->i + 0.5);
	set_fvec(&player_direction, -1, 0);
	set_fvec(&player_view_plain, 0, (double)g->wnd.i / (double)g->wnd.j / 2.0);
	rot = 0;
	if (g->map[p_pos->i][p_pos->j] == 'S')
		rot = M_PI;
	else if (g->map[p_pos->i][p_pos->j] == 'E')
		rot = -M_PI_2;
	else if (g->map[p_pos->i][p_pos->j] == 'W')
		rot = M_PI_2;
	rotation(&player_direction, rot);
	rotation(&player_view_plain, rot);
	g->ppos.x = player_pos.y;
	g->ppos.y = player_pos.x;
	g->pdx = player_direction.x;
	g->pdy = player_direction.y;
	g->planex = player_view_plain.x;
	g->planey = player_view_plain.y;
}

void map_check(t_god *g)
{
	t_ivec index_v;
	t_ivec p_cell;
	char **for_close_check;

	for_close_check = malloc(sizeof(char *) * (g->map_h));
	index_v.i = -1;
	while(++index_v.i < g->map_h)
	{
		*(for_close_check + index_v.i) = ft_strdup(*(g->map + index_v.i));
		index_v.j = -1;
		while(++index_v.j < g->map_w)
			if(ft_strchr("NSWE", g->map[index_v.i][index_v.j]))
			{
				set_ivec(&p_cell, index_v.i, index_v.j);
				set_start_pos(g, &p_cell);
			}
	}
	map_closecheck(p_cell, for_close_check, g);
	free_2d((void **)for_close_check, g->map_h);
	return ;
}

int set_err_msg(t_god *g, char *msg)
{
	char *free_tmp;

	if (!g->err_msg)
		g->err_msg = ft_strdup("ERROR\n");
	if (g->err_msg)
	{
		free_tmp = g->err_msg;
		g->err_msg = ft_strjoin(g->err_msg, msg);
		free(free_tmp);
	}
	return (0);
}

void read_color(t_god *g, char *str, int line_count)
{
	int *target_p;
	int red;
	int green;
	int blue;

	red = 256;
	green = 256;
	blue = 256;
	target_p = *str == 'C' ? &g->ce_rgb : &g->fl_rgb;
	*str++;
	read_nmb(&str, &red, g->scr_y, 256);
	str += *str == ',' ? 1 : 0;
	read_nmb(&str, &green, g->scr_y, 256);
	str += *str == ',' ? 1 : 0;
	read_nmb(&str, &blue, g->scr_y, 256);
	if (*target_p != -1)
		set_err_msg(g, "The cub file has double setting in ceiling or floor color.\n");
	if (255 < red || 255 < green || 255 < blue || *str)
		set_err_msg(g, "The cub file has inaccseptable setting in ceiling or floor color.\n");
	*target_p = (red << 16 | green << 8 | blue);
}

void read_img(t_god *g, char *str, int line_count)
{
	char *f_name;
	t_img *target;
	int start_pos;

	start_pos = 2;	
	if (!ft_memcmp(str, "S ", 2))
	{
		target = &g->s_img;
		start_pos = 1;	
	}
	else if (!ft_memcmp(str, "SO ", 3))
		target = &g->so_img;
	else if (!ft_memcmp(str, "WE ", 3))
		target = &g->we_img;
	else if (!ft_memcmp(str, "NO ", 3))
		target = &g->no_img;
	else if (!ft_memcmp(str, "EA ", 3))
		target = &g->ea_img;

	str += start_pos;
	f_name = ft_strtrim(str, " \t\n\v\f\r");
	if (target->p)
	{
		set_err_msg(g, "Texture configration [");
		set_err_msg(g, str);
		set_err_msg(g, "] is not first one.\n");
		free(f_name);
		return;
	}
	target->p = mlx_xpm_file_to_image(g->mlx, f_name, &target->x_size, &target->y_size);
	if (!target->p)
	{
		set_err_msg(g, "Texture file [");
		set_err_msg(g, f_name);
		set_err_msg(g, "] is not able to load.\n");
	}
	else
		target->addr = (int *)mlx_get_data_addr(target->p, &target->bpp, &target->llen, &target->endian);
	free(f_name);
}

void make_mapdata(t_god *g)
{
	int i;
	t_list *list_p;

	list_p = g->map_list;

	g->map = malloc(sizeof(char *) * (g->map_h + 1));
	*(g->map + g->map_h) = 0;
	i = 0;
	while (i < g->map_h)
	{
		*(g->map + i) = malloc(sizeof(char) * (g->map_w + 1));
		ft_memset(*(g->map + i), ' ', g->map_w);
		ft_memcpy(*(g->map + i), list_p->content, ft_strlen(list_p->content));
		*(*(g->map + i) + g->map_w) = 0;
		list_p = list_p->next;
		i++;
	}
}

void interpret_line(t_god *g, char *line, int line_count)
{
	char *temp_str;
	char *non_map_parts_tmp;
	t_list *map_parts;

	temp_str = ft_strtrim(line, " \t\n\v\f\r");
	non_map_parts_tmp = ft_strtrim(line, " 012NSWE");
	
	if(!g->map && !g->map_list)
	{
		if(ft_strlen(temp_str) && !ft_memcmp(temp_str, "R ", 2))
			read_r(g, temp_str, line_count);
		else if(ft_strlen(temp_str) && !ft_memcmp(temp_str, "C ", 2) || !ft_memcmp(temp_str, "F ", 2))
			read_color(g, temp_str, line_count);
		else if(ft_strlen(temp_str) && !ft_memcmp(temp_str, "SO ", 3) ||
								!ft_memcmp(temp_str, "NO ", 3) ||
								!ft_memcmp(temp_str, "WE ", 3) ||
								!ft_memcmp(temp_str, "EA ", 3) ||
								!ft_memcmp(temp_str, "S ", 2))
			read_img(g, temp_str, line_count);
		else if(ft_strlen(temp_str) && !ft_strlen(non_map_parts_tmp))
		{
			ft_lstadd_back(&g->map_list, ft_lstnew(ft_strdup(line)));
			g->map_h += 1;
			g->map_w = ft_strlen(line);
		}
	}
	else if(!g->map)
	{
		if(!ft_strlen(temp_str))
		{
			make_mapdata(g);
			map_check(g);
		}
		else if(ft_strlen(non_map_parts_tmp))
		{
			set_err_msg(g, "[");
			set_err_msg(g, line);
			set_err_msg(g, "] contein unmap object char\n");
		}
		else
		{
			ft_lstadd_back(&g->map_list, ft_lstnew(ft_strdup(line)));
			g->map_h += 1;
			g->map_w = g->map_w < ft_strlen(line) ? ft_strlen(line) : g->map_w;
		}
	}
	else
		if(ft_strlen(temp_str))
			set_err_msg(g, "It is fobbited that put any describing after map\n");
	free(temp_str);
	free(non_map_parts_tmp);
}

/*
** If it do free(g->mlx), the malloced memory will be unrecable in kept in mxl_init,
** so this program is not free the mlx pointer.
*/
void destroy_god(t_god *g)
{
	if (g->s_img.p)
		mlx_destroy_image(g->mlx, g->s_img.p);
	if (g->ea_img.p)
		mlx_destroy_image(g->mlx, g->ea_img.p);
	if (g->we_img.p)
		mlx_destroy_image(g->mlx, g->we_img.p);
	if (g->so_img.p)
		mlx_destroy_image(g->mlx, g->so_img.p);
	if (g->no_img.p)
		mlx_destroy_image(g->mlx, g->no_img.p);
	if (g->w_img.p)
		mlx_destroy_image(g->mlx, g->w_img.p);
	if (g->win)
		mlx_destroy_window(g->mlx, g->win);
	if (g->map_list)
		ft_lstclear(&g->map_list, &free);
	while(g->map_h + 1 && g->map)
		free(*(g->map + g->map_h--));
	free(g->map);
	free(g->mlx);
	free(g->title);
	free(g->err_msg);
	free(g->cub_fname);
	free(g->bmp_fname);
	ft_bzero(g, sizeof(t_god));
}

int load_settings(t_god *g, int argc, char **argv)
{
	int cub_f_len;
	int fd;
	char *line;
	int line_count;

	if (argc < 2 || 3 < argc)
		return (set_err_msg(g, "Please specify the argument correctly.\n"));
	if ((cub_f_len = ft_strlen(argv[1])) <= 4 || ft_strncmp(argv[1] + cub_f_len - 4, ".cub", 4))
		return (set_err_msg(g, "please set '.cub' file.\n"));
	if(argc == 3 && ft_strncmp(argv[2], "--save", ft_strlen(argv[2])))
		return (set_err_msg(g, "plsese use \"--save\" in 2nd args\n"));
	g->cub_fname = ft_strdup(argv[1]);
	if(0 > (fd = open(argv[1], O_RDONLY)))
		return (set_err_msg(g, "the '.cub' file is not exist!\n"));
	line_count = 0;
	while(0 < get_next_line(fd, &line))
	{
		interpret_line(g, line, ++line_count);
		free(line);
	}
	interpret_line(g, line, ++line_count);
	free(line);
	line = ft_strdup("");
	interpret_line(g, line, line_count);
	free(line);
	if(argc == 3)
		g->bmp = 1;
}

void exit_func(t_god *g)
{
	destroy_god(g);
	exit(0);
}

void  next_pl(t_god *g)
{
	t_fvec step;
	double wdist;
	
	step.x = g->pdx * g->moveSpeed_ga + g->pdy * g->moveSpeed_sw;
	step.y = g->pdy * g->moveSpeed_ga - g->pdx * g->moveSpeed_sw;
	wdist = 0 < step.x ? WALLDIST : -WALLDIST;
	if(g->map[(int)(g->ppos.x + step.x + wdist)][(int)(g->ppos.y)] == '1')
		step.x = 0;
	wdist = 0 < step.y ? WALLDIST : -WALLDIST;
	if(g->map[(int)(g->ppos.x)][(int)(g->ppos.y + step.y + wdist)] == '1')
		step.y = 0;
	g->ppos.x += step.x;
	g->ppos.y += step.y;
}

void next_plane(t_god *g)
{
	double oldx;

	oldx = g->planex;
	g->planex = g->planex * cos(g->rotSpeed) - g->planey * sin(g->rotSpeed);
	g->planey = oldx * sin(g->rotSpeed) + g->planey * cos(g->rotSpeed);
}

void next_pd(t_god *g)
{
	double oldpdx = g->pdx;
	g->pdx = g->pdx * cos(g->rotSpeed) - g->pdy * sin(g->rotSpeed);
	g->pdy = oldpdx * sin(g->rotSpeed) + g->pdy * cos(g->rotSpeed);
}

void write_imgf(t_god *g);

int loop_func(t_god *g)
{
	next_pd(g);
	next_plane(g);
	next_pl(g);

	paint_bg(g);
	make_image(g);
	if (g->exit)
		exit_func(g);
	else if(g->bmp)
	{
		write_imgf(g);
		exit_func(g);
	}
	else
		mlx_put_image_to_window(g->mlx, g->win, g->w_img.p, 0, 0);
	mlx_do_sync(g->mlx);
	return (0);
}

void paint_bg(t_god *g)
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

void verLine(int x, int drowstart, int drowend, t_god *g, double tx, t_img *im)
{
	int y;
	y = drowstart;
	unsigned int color;
	int txp;
	int typ;

	txp = (int)(tx * (double)(im->x_size));

	while (y <= drowend)
	{
		typ = (int)((double)(y - drowstart) / (double)(drowend - drowstart) * (double)(im->y_size));
		color = *((unsigned int *)(im->addr + typ * im->llen) + txp);
		if (0 <= y && y < g->wnd.j)
			my_mlx_pixel_put(g, x, y, color);
		y++;
	}
}

void verLine2(int x, t_god *g, int *mx)
{
	int i;
	double spriteX;
	double spriteY;
	double invDet;
	double transformX;
	double transformY;
	unsigned int color;
	int ymask[g->wnd.j];

	ft_bzero(ymask, sizeof(int) * g->wnd.j);

	i = 0;
	while(mx[2 * i])
	{
		spriteX = (double)mx[2 * i] + 0.5 - g->ppos.x;
		spriteY = (double)mx[2 * i + 1] + 0.5 - g->ppos.y;
		
		invDet = 1.0 / (g->planex * g->pdy - g->pdx * g->planey);

		transformX = invDet * (g->pdy * spriteX - g->pdx * spriteY);
		transformY = invDet * (-g->planey * spriteX + g->planex * spriteY);
		
		int spriteScreenX = (int)((g->wnd.i / 2) * (1 + transformX / transformY));
		int spriteHeight = ABS((int)(g->wnd.j/(transformY)));
		int drawStartY = - spriteHeight / 2 + g->wnd.j / 2;
		if(drawStartY < 0)
			drawStartY = 0;
		int drawEndY = spriteHeight / 2 + g->wnd.j / 2;
		if(drawEndY >= g->wnd.j)
			drawEndY = g->wnd.j - 1;
		int spriteWidth = abs((int)(g->wnd.j/ (transformY)));
		int drawStartX = -spriteWidth / 2 + spriteScreenX;
		if(drawStartX < 0) drawStartX = 0;
		int drawEndX = spriteWidth / 2 + spriteScreenX;
		if(drawEndX >= g->wnd.i) drawEndX = g->wnd.i - 1;
		if (drawStartX <= x && x < drawEndX)
		{
			int texX = (int) (256 * (x - (-spriteWidth / 2 + spriteScreenX)) * g->s_img.x_size / spriteWidth) / 256;
			if(transformY > 0 && x > 0 && x < g->wnd.i);
			{
				int y;
				y = drawStartY;
				while(y < drawEndY)
				{
					int d = (y) * 256 - g->wnd.j * 128 + spriteHeight * 128;
					int texY = ((d * g->s_img.y_size) / spriteHeight) / 256;
					color = *((unsigned int *)(g->s_img.addr + texY * g->s_img.llen) + texX);
					if((color & 0x00FFFFFF) != 0 && ymask[y] == 0)
					{
						ymask[y] = 1;
						my_mlx_pixel_put(g, x, y, color);
					}
					y++;
				}
			}
		}
		i++;
	}
}

void make_image(t_god *g)
{
	int x;
	int mapX;
	int mapY;
	int *mx;

	mx = ft_calloc(sizeof(int), MAX(g->map_h, g->map_w) * 4);

	x = 0;
	while(x < g->wnd.i)
	{
		ft_bzero(mx, MAX(g->map_h, g->map_w) * 4 * sizeof(int));
		double cameraX = 2 * x / (double)(g->wnd.i) - 1;
		double rayDirX = g->pdx + g->planex * cameraX;
		double rayDirY = g->pdy + g->planey * cameraX;
		mapX = (int)(g->ppos.x);
		mapY = (int)(g->ppos.y);

		double sideDistX;
		double sideDistY;
		double deltaDistX = ABS(1 / rayDirX);
		double deltaDistY = ABS(1 / rayDirY);
		double perpWallDist;
		int stepX;
		int stepY;

		int hit = 0;
		int side;
		if(rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (g->ppos.x - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - g->ppos.x) * deltaDistX;
		}
		if(rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (g->ppos.y - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - g->ppos.y) * deltaDistY;
		}
		int mxi = 0;
		while (hit == 0)
		{
			if(sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			if(g->map[mapX][mapY] == '1')
				hit = 1;
			else if (g->map[mapX][mapY] == '2')
			{
				mx[mxi * 2] = mapX;
				mx[mxi * 2 + 1] = mapY;
				mxi++;
			}	
		}
		if(side == 0)
			perpWallDist = (mapX - g->ppos.x + (1 - stepX) / 2) / rayDirX;
		else
			perpWallDist = (mapY - g->ppos.y + (1 - stepY) / 2) / rayDirY;
		int lineHeight = (int)(g->wnd.j / perpWallDist);

		int drawStart;
		drawStart = -lineHeight / 2 + g->wnd.j / 2;
		int drawEnd;
		drawEnd = lineHeight / 2 + g->wnd.j / 2;

		t_img *texture_img;
		double tx;
		if(side == 0)
		{
			tx = g->ppos.y + perpWallDist * rayDirY;
			texture_img = &g->no_img;
		}
		else
		{
			tx = g->ppos.x + perpWallDist * rayDirX;
			texture_img = &g->ea_img;
		}
		tx -= floor(tx);
		if(side == 0 && rayDirX > 0)
		{
			tx = 1 - tx;
			texture_img = &g->so_img;
		}
		if(side == 1 && rayDirY < 0)
		{
			tx = 1 - tx;
			texture_img = &g->we_img;
		}
		//draw the pixels of the stripe as a vertical line
		verLine(x, drawStart, drawEnd, g, tx, texture_img);
		verLine2(x, g, mx);
		x++;
	}

	free(mx);
}

int hook_keypress_func(int key_code, t_god *g)
{
	if (key_code == KEY_CODE_W)
		g->moveSpeed_ga += MOVESPEED;
	if (key_code == KEY_CODE_S)
		g->moveSpeed_ga -= MOVESPEED;
	if (key_code == KEY_CODE_D)
		g->moveSpeed_sw += MOVESPEED;
	if (key_code == KEY_CODE_A)
		g->moveSpeed_sw -= MOVESPEED;
	if (key_code == KEY_CODE_LA)
		g->rotSpeed += ROTSPEED;
	if (key_code == KEY_CODE_RA)
		g->rotSpeed -= ROTSPEED;
	if (key_code == KEY_CODE_UA)
		;
	if (key_code == KEY_CODE_DA)
		;
	if (key_code == KEY_CODE_ESC)
		g->exit = 1;
	return (0);
}

int hook_keyrelease_func(int key_code, t_god *g)
{
	g->plv += 1.0;
	if (key_code == KEY_CODE_W)
		g->moveSpeed_ga -= MOVESPEED;
	if (key_code == KEY_CODE_S)
		g->moveSpeed_ga += MOVESPEED;
	if (key_code == KEY_CODE_D)
		g->moveSpeed_sw -= MOVESPEED;
	if (key_code == KEY_CODE_A)
		g->moveSpeed_sw += MOVESPEED;
	if (key_code == KEY_CODE_LA)
		g->rotSpeed -= ROTSPEED;
	if (key_code == KEY_CODE_RA)
		g->rotSpeed += ROTSPEED;
	if (key_code == KEY_CODE_UA)
		;
	if (key_code == KEY_CODE_DA)
		;
	return (0);
}

int hook_exit_func(t_god *g)
{
	g->exit = 1;
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
		g.title = ft_strdup(argv[1]);
		mlx_do_key_autorepeatoff(g.mlx);
		g.w_img.p = mlx_new_image(g.mlx, g.wnd.i, g.wnd.j);
		g.w_img.addr = mlx_get_data_addr(g.w_img.p, &g.w_img.bpp, &g.w_img.llen, &g.w_img.endian);
		if(!g.bmp)
		{
			g.win = mlx_new_window(g.mlx, g.wnd.i, g.wnd.j, g.title);
			mlx_hook(g.win, 0b10, 0b11, &hook_keypress_func, &g);
			mlx_hook(g.win, 0b11, 0b10, &hook_keyrelease_func, &g);
			mlx_hook(g.win, 17, 0x20000, &hook_exit_func, &g);
			mlx_loop_hook(g.mlx, &loop_func, &g);
			mlx_loop(g.mlx);
		}
		loop_func(&g);
		return (0);
	}
}

void	littleendian(unsigned char *s, unsigned int n, int size)
{
	int i;
	
	i = 0;
	s[i++] = (unsigned char)(n);
	while(i < size)
		s[i++] = (unsigned char)(n >>= 8);
}

void	write_header(int fd, t_god *g)
{
	unsigned char	header[BMP_HEADER_SIZE + DIB_HEADER_SIZE];

	ft_bzero(header, BMP_HEADER_SIZE + DIB_HEADER_SIZE);
	ft_memcpy(header, "BM", 2);
	littleendian(header + 2, BMP_HEADER_SIZE + DIB_HEADER_SIZE +
		(BMP_BPP * g->wnd.i * g->wnd.j), 4);
	littleendian(header + 10, BMP_HEADER_SIZE + DIB_HEADER_SIZE, 4);
	littleendian(header + 14, DIB_HEADER_SIZE, 4);
	littleendian(header + 18, g->wnd.i, 4);
	littleendian(header + 22, g->wnd.j, 4);
	littleendian(header + 26, BMP_NUM_PLANES, 2);
	littleendian(header + 28, BMP_BPP * 8, 2);
	write(fd, header, BMP_HEADER_SIZE + DIB_HEADER_SIZE);
}

int		write_pixels(int fd, t_god *g)
{
	t_ivec			it;
	unsigned char	*line;

	if (!(line = ft_calloc(sizeof(char), g->wnd.i * BMP_BPP)))
		return (-1);
	it.i = g->wnd.j;
	while (--it.i >= 0)
	{
		it.j = -1;
		while (++it.j < g->wnd.i)
			littleendian(line + (it.j * BMP_BPP), 
			*((unsigned int *)(g->w_img.addr + it.i * g->w_img.llen) + it.j), 4);
		write(fd, line, g->wnd.i * BMP_BPP);
	}
	free(line);
	return (0);
}

void			write_imgf(t_god *g)
{
	int		fd;

	if (!(g->bmp_fname = ft_strdup(g->cub_fname)))
	{
		set_err_msg(g, "memory alocate error in making bmp file.");
		return ;
	}
	ft_memcpy(g->bmp_fname + ft_strlen(g->bmp_fname) - 3, "bmp", 3);
	if ((fd = open(g->bmp_fname, O_CREAT | O_WRONLY, 0666)) == -1)
	{
		set_err_msg(g, "file open error in making bmp file.");
		return ;
	}
	write_header(fd, g);
	if (write_pixels(fd, g) == -1)
		set_err_msg(g, "Failed to open bmp file\n");
	close(fd);
	return ;
}