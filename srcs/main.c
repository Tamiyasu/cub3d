#include "cub3d.h"

#define KEY_ESC			53
#define X_EVENT_KEY_PRESS 2
#define X_EVENT_KEY_RELEASE 3
#define X_EVENT_KEY_EXIT 17

void init_god(t_god *g)
{
    ft_bzero(g, sizeof(t_god));
	g->ce_rgb = -1;
	g->fl_rgb = -1;
	g->mlx = mlx_init();
	if(!g->mlx)
	{
		set_err_msg(g, "Please check your XWINDOW settings\n");
		return;
	}	
	mlx_get_screen_size(g->mlx, &g->scr_x, &g->scr_y);
}

void print_game(t_god *g)
{
	int i;
	printf("mlx : %p / ", g->mlx);
    printf("win : %p / ", g->win);
	printf("w_img : %p / ", g->w_img.p);
	printf("wnd_y : %d / ", g->wnd_y);
	printf("wnd_x : %d / ", g->wnd_x);
	printf("scr_y : %d / ", g->scr_y);
	printf("scr_x : %d / ", g->scr_x);
	printf("title : %p [%s] / ", g->title, g->title);
	printf("ea_img : %p / ", g->ea_img.p);
	printf("we_img : %p / ", g->we_img.p);
	printf("so_img : %p / ", g->so_img.p);
	printf("no_img : %p / ", g->no_img.p);
	printf("s_img : %p / " , g->s_img.p);
	printf("ce_rgb : %d / ", g->ce_rgb);
	printf("fl_rgb : %d / ", g->fl_rgb);
	printf("map : %p / ", g->map);
	printf("map_h : %d / ", g->map_h);
	printf("map_w : %d / ", g->map_w);
	printf("bmp : %d / ", g->bmp);
	printf("err_msg : %p [%s] / ", g->err_msg, g->err_msg);
	printf("\n");
	i = 0;
	while(i < g->map_h && g->map)
		printf("[%s]\n", *(g->map + i++));
	printf("\n");
}


void            my_mlx_pixel_put(t_god *g, int x, int y, int color)
{
    char    *dst;

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
	if (g->wnd_x || g->wnd_y)
	{
		set_err_msg(g, "The cub file has double setting in window size.\n");
		return;
	}
	str++;
	read_nmb(&str, &g->wnd_x, g->scr_x, 0);
	read_nmb(&str, &g->wnd_y, g->scr_y, 0);
	if (*str)
	{
		g->wnd_y = 0;
		g->wnd_x = 0;
	}
	if (!g->wnd_x || !g->wnd_y)
	{
		set_err_msg(g, "The cub file has wrong setting in window size.\n");
		return;
	}
}


void set_r_on_map(char **map, int i, int j, t_list **lst_last)
{
	t_pos *tmp;

	if (map[i][j] != '1' && map[i][j] != 'R') 
	{
		map[i][j] = 'R';
		tmp = malloc(sizeof(t_pos));
		tmp->i = i;
		tmp->j = j;
		(*lst_last)->next = ft_lstnew(tmp);
		*lst_last = (*lst_last)->next;
	}
}

void ft_lstdelhead(t_list **lst, void (*del)(void *))
{
	t_list *lst_tmp;

	lst_tmp = *lst;
	*lst = (*lst)->next;
	ft_lstdelone(lst_tmp, del);
}

void map_closecheck(int i, int j, char **map, t_god *g)
{
	t_list *lst;
	t_list *lst_last;
	t_pos *pos;

	lst = NULL;
	lst_last = NULL;
	pos = malloc(sizeof(t_pos));
	pos->i = i;
	pos->j = j;
	map[i][j] = 'R';

	lst = ft_lstnew(pos);
	lst_last = lst;
	while(lst)
	{
		i = ((t_pos *)(lst->content))->i;
		j = ((t_pos *)(lst->content))->j;
		if (i == 0 || i == g->map_h - 1 || j == 0 || j == g->map_w - 1 || map[i][j - 1] == ' ' || map[i][j + 1] == ' ' || map[i - 1][j] == ' ' || map[i + 1][j] == ' ')
		{
			set_err_msg(g, "map is not closed.\n");
			ft_lstclear(&lst, &free);
			return;
		}
		set_r_on_map(map, i, j - 1, &lst_last);
		set_r_on_map(map, i, j + 1, &lst_last);
		set_r_on_map(map, i - 1, j, &lst_last);
		set_r_on_map(map, i + 1, j, &lst_last);
		ft_lstdelhead(&lst, &free);
	}
}

/* set_err_msg if map have issue (not closed or have no player or multiple players*/
void map_check(t_god *g)
{
	int i;
	int j;
	int p_pos_h;
	int p_pos_w;
	char **for_close_check;

	for_close_check = malloc(sizeof(char *) * (g->map_h));
	i = -1;
	while(++i < g->map_h)
	{
		*(for_close_check + i) = ft_strdup(*(g->map + i));
		j = -1;
		while(++j < g->map_w)
			if(ft_strchr("NSWE", g->map[i][j]))
			{
				//printf("pos : [%d, %d], player_c : %c\n", i, j, g->map[i][j]);
				p_pos_h = i;
				p_pos_w = j;
				g->pli = i + 0.5;
				g->plj = j + 0.5;
				g->pdx = -1;
				g->pdy = 0;
				g->planex = 0;
				g->planey = 0.66;
				if (g->map[i][j] == 'N')
					g->plv = 0.0;
				else if (g->map[i][j] == 'E')
					g->plv = 0.25;
				else if (g->map[i][j] == 'S')
					g->plv = 0.5;
				else if (g->map[i][j] == 'W')
					g->plv = 0.75;
			}
	}
	map_closecheck(p_pos_h, p_pos_w, for_close_check, g);
	i = -1;
	while(++i < g->map_h)
		free(*(for_close_check + i));
	free(for_close_check);
	return ;
}

void set_err_msg(t_god *g, char *msg)
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
	{

	}
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
	ft_bzero(g, sizeof(t_god));
}

void load_settings(t_god *g, int argc, char **argv)
{
	int cub_f_len;
	int fd;
	char *line;
	int line_count;

	if (argc < 2 || 3 < argc)
	{
		set_err_msg(g, "Please specify the argument correctly.\n");
		return ;
	}
    if ((cub_f_len = ft_strlen(argv[1])) <= 4 || ft_strncmp(argv[1] + cub_f_len - 4, ".cub", 4))
		set_err_msg(g, "please set '.cub' file.\n");
	else
	{
		if(0 > (fd = open(argv[1], O_RDONLY)))
		{
			set_err_msg(g, "the '.cub' file is not exist!\n");
			return;
		}
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
	}
	if(argc == 3 && ft_strncmp(argv[2], "--save", ft_strlen(argv[2])))
		set_err_msg(g, "plsese use \"--save\" in 2nd args\n");
}

void exit_func(t_god *g)
{
	destroy_god(g);
	exit(0);
}
void	make_image(t_god *g);

int loop_func(t_god *g)
{
	if (g->exit)
		exit_func(g);
	mlx_do_sync(g->mlx);
	g->plv += 1.0;
	int i;
	int j;
	i = 0;
	while (i < g->wnd_x)
	{	
		j = 0;
		while (j < g->wnd_y)
		{
			if (j < g->wnd_y / 2)
				my_mlx_pixel_put(g, i, j, g->ce_rgb);
			else
				my_mlx_pixel_put(g, i, j, g->fl_rgb);
			j++;
		}
		i++;
	}
	make_image(g);
	mlx_put_image_to_window(g->mlx, g->win, g->w_img.p, 0, 0);
//	sleep(3); //please remove before eval.
	return (0);
}

void verLine(int x, int drowstart, int drowend, int color, t_god *g)
{
	int y;
	y = drowstart;

	while (y <= drowend) 
		my_mlx_pixel_put(g, x, y++, color);
}

void make_image(t_god *g)
{
	int x;
	int mapX;
	int mapY;

	x = 0;
	while(x < g->wnd_x)
	{
		double cameraX = 2 * x / (double)(g->wnd_x) - 1;
		double rayDirX = g->pdx + g->planex * cameraX;
		double rayDirY = g->pdy + g->planey * cameraX;
		//printf("g->pli : %f, g->plj : %f\n", g->pli, g->plj);
		//printf("(int)i : %d, (int)j : %d\n", (int)g->pli, (int)g->plj);
		mapX = (int)(g->pli);
		mapY = (int)(g->plj);
		//printf("mapX : %d, mapY : %d\n", mapX, mapY);

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
        	sideDistX = (g->pli - mapX) * deltaDistX;
	    }
	    else
		{
        	stepX = 1;
	        sideDistX = (mapX + 1.0 - g->pli) * deltaDistX;
      	}
      	if(rayDirY < 0)
      	{
			stepY = -1;
			sideDistY = (g->plj - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - g->plj) * deltaDistY;
		}
		while (hit == 0)
		{
	        //jump to next map square, OR in x-direction, OR in y-direction
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
    	    //Check if ray has hit a wall
			//printf("mapX : %d, mapY : %d, pos : %d\n", mapX, mapY, g->map[mapX][mapY]);
        	if(g->map[mapX][mapY] == '1')
				hit = 1;
	    }
     	//Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
      	if(side == 0) 
			perpWallDist = (mapX - g->pli + (1 - stepX) / 2) / rayDirX;
     	else
		  perpWallDist = (mapY - g->plj + (1 - stepY) / 2) / rayDirY;
	    //Calculate height of line to draw on screen
    	int lineHeight = (int)(g->wnd_y / perpWallDist);

      //calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + g->wnd_y / 2;
		int drawEnd;
		if(drawStart < 0)drawStart = 0;
			drawEnd = lineHeight / 2 + g->wnd_y / 2;
		if(drawEnd >= g->wnd_y)
			drawEnd = g->wnd_y - 1;

      //choose wall color
		int color;
		switch(g->map[mapX][mapY])
		{
			case 1:  color = 0x00ff0000;    break; //red
			case 2:  color = 0x0000ff00;  break; //green
			case 3:  color = 0x000000ff;   break; //blue
			case 4:  color = 0x00ffffff;  break; //white
			default: color = 0x00ffff00; break; //yellow
		}

		//give x and y sides different brightness
		if(side == 1) {color = color / 2;}

		//draw the pixels of the stripe as a vertical line
		verLine(x, drawStart, drawEnd, color, g);
/*
    //timing for input and FPS counter
    oldTime = time;
    time = getTicks();
    double frameTime = (time - oldTime) / 1000.0; //frameTime is the time this frame has taken, in seconds
    print(1.0 / frameTime); //FPS counter
    redraw();
    cls();

    //speed modifiers
    double moveSpeed = frameTime * 5.0; //the constant value is in squares/second
    double rotSpeed = frameTime * 3.0; //the constant value is in radians/second
    readKeys();
    //move forward if no wall in front of you
    if(keyDown(SDLK_UP))
    {
      if(worldMap[int(posX + dirX * moveSpeed)][int(posY)] == false) posX += dirX * moveSpeed;
      if(worldMap[int(posX)][int(posY + dirY * moveSpeed)] == false) posY += dirY * moveSpeed;
    }
    //move backwards if no wall behind you
    if(keyDown(SDLK_DOWN))
    {
      if(worldMap[int(posX - dirX * moveSpeed)][int(posY)] == false) posX -= dirX * moveSpeed;
      if(worldMap[int(posX)][int(posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;
    }
    //rotate to the right
    if(keyDown(SDLK_RIGHT))
    {
      //both camera direction and camera plane must be rotated
      double oldDirX = dirX;
      dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
      dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
      planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
    //rotate to the left
    if(keyDown(SDLK_LEFT))
    {
      //both camera direction and camera plane must be rotated
      double oldDirX = dirX;
      dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
      dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
      planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }
  }
*/


		x++;
	}
}


int hook_keypress_func(int key_code, t_god *g)
{
	printf("here - press\n");
	printf("key code is : %d\n", key_code);
	if (key_code == 65307)
		g->exit = 1;
	return (0);
}

int hook_keyrelease_func(int key_code, t_god *g)
{
	printf("here -release\n");
	printf("key code is : %d\n", key_code);
	print_game(g);
	printf("in key hook p->plv : %f\n", g->plv);
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
		g.title = ft_strdup("test");
		printf("game p : %p\n", &g);
		print_game(&g);
		g.win = mlx_new_window(g.mlx, g.wnd_x, g.wnd_y, g.title);
		//*
		g.w_img.p = mlx_new_image(g.mlx, g.wnd_x, g.wnd_y);
		g.w_img.addr = mlx_get_data_addr(g.w_img.p, &g.w_img.bpp, &g.w_img.llen, &g.w_img.endian);

		print_game(&g);

	    mlx_hook(g.win, 0b10, 0b11, &hook_keypress_func, &g);
    	mlx_hook(g.win, 0b11, 0b10, &hook_keyrelease_func, &g);
		mlx_hook(g.win, 17, 0x20000, &hook_exit_func, &g);
		mlx_loop_hook(g.mlx, &loop_func, &g);
		mlx_loop(g.mlx);
	}
	//*/
}
//gcc -I ~/42/minilibx_linux/ main.c ~/42/minilibx_linux/*.a  -Imlx_linux -lXext -lX11 -lm -lz 