#include "cub3d.h"

void init_god(t_god *g)
{
    ft_bzero(g, sizeof(t_god));
	g->ce_rgb = -1;
	g->fl_rgb = -1;
	g->mlx = mlx_init();
	mlx_get_screen_size(g->mlx, &g->scr_x, &g->scr_y);
}

void print_game(t_god *g)
{
	printf("mlx : %p / ", g->mlx);
    printf("win : %p / ", g->win);
	printf("win_img : %p / ", g->win_img);
	printf("img_addr : %p / ", g->img_addr);
	printf("img_bits_per_pixel : %d / ", g->img_bits_per_pixel);
	printf("img_line_length : %d / ", g->img_line_length);
	printf("img_endian : %d / ", g->img_endian);
	printf("wnd_y : %d / ", g->wnd_y);
	printf("wnd_x : %d / ", g->wnd_x);
	printf("scr_y : %d / ", g->scr_y);
	printf("scr_x : %d / ", g->scr_x);
	printf("title : %p [%s] / ", g->title, g->title);
	printf("eo_fpath : %p [%s] / ", g->eo_fpath, g->eo_fpath);
	printf("wo_fpath : %p [%s] / ", g->wo_fpath, g->wo_fpath);
	printf("so_fpath : %p [%s] / ", g->so_fpath, g->so_fpath);
	printf("no_fpath : %p [%s] / ", g->no_fpath, g->no_fpath);
	printf("sp_fpath : %p [%s] / ", g->sp_fpath, g->sp_fpath);
	printf("ce_rgb : %d / ", g->ce_rgb);
	printf("fl_rgb : %d / ", g->fl_rgb);
	printf("map : %p / ", g->map);
	printf("map_h : %d / ", g->map_h);
	printf("map_w : %d / ", g->map_w);
	printf("bmp : %d / ", g->bmp);
	printf("err_msg : %p [%s] / ", g->err_msg, g->err_msg);
	printf("\n");
}


void            my_mlx_pixel_put(t_god *data, int x, int y, int color)
{
    char    *dst;

    dst = data->img_addr + (y * data->img_line_length + x * (data->img_bits_per_pixel / 8));
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

void read_r(t_god *g, char *line, int line_count)
{
	if (g->wnd_x || g->wnd_y)
	{
		g->err_msg = ft_strdup("The cub file has double setting in window size.");
		return;
	}
	line++;
	read_nmb(&line, &g->wnd_x, g->scr_x, 0);
	read_nmb(&line, &g->wnd_y, g->scr_y, 0);
	if (*line)
	{
		g->wnd_y = 0;
		g->wnd_x = 0;
	}
	if (!g->wnd_x || !g->wnd_y)
	{
		g->err_msg = ft_strdup("The cub file has wrong setting in window size.");
		return;
	}
}

void interpret_line(t_god *g, char *line, int line_count)
{
	if(ft_strlen(line) && !ft_memcmp(line, "R ", 2))
		read_r(g, line, line_count);
	else if(ft_strlen(line) && !ft_memcmp(line, "C ", 2) || !ft_memcmp(line, "F ", 2))
	{
		int *target_p;
		int red;
		int green;
		int blue;

		red = 256;
		green = 256;
		blue = 256;
		target_p = *line == 'C' ? &g->ce_rgb : &g->fl_rgb;
		line++;
		read_nmb(&line, &red, g->scr_y, 256);
		line += *line == ',' ? 1 : 0;
		read_nmb(&line, &green, g->scr_y, 256);
		line += *line == ',' ? 1 : 0;
		read_nmb(&line, &blue, g->scr_y, 256);
		if (*target_p != -1)
			g->err_msg = ft_strdup("The cub file has double setting in ceiling or floor color.");
		else if (255 < red || 255 < green || 255 < blue || *line || g->err_msg)
			g->err_msg = ft_strdup("The cub file has inaccseptable setting in ceiling or floor color.");
		*target_p = (red << 16 | green << 8 | blue);
		print_game(g);
	}
//	else
//		printf("No\n");
}

void load_settings(t_god *g, int argc, char **argv)
{
	int cub_f_len;
	int fd;
	char *line;
	int line_count;

	if (argc < 2 || 3 < argc)
	{
		g->err_msg = ft_strdup("Please specify the argument correctly.");
		return;
	}
    if ((cub_f_len = ft_strlen(argv[1])) <= 4 || ft_strncmp(argv[1] + cub_f_len - 4, ".cub", 4))
	{
		g->err_msg = ft_strdup("please set '.cub' file.");
		return;
	}
	else
	{
		if(0 > (fd = open(argv[1], O_RDONLY)))
		{
			g->err_msg = ft_strdup("the '.cub' file is not exist!");
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
		//if (!g->err_msg)
		//	g->err_msg = ft_strdup("the config file reading function is not imprimented yet");
		return;
	}
	
}


int	main(int argc, char **argv)
{
	t_god	g;

	init_god(&g);
	load_settings(&g, argc, argv);
	if (g.err_msg)
	{
		write(1, "ERROR\n", 6);
		write(1, g.err_msg, ft_strlen(g.err_msg));
		write(1, "\n", 1);
	}
	else
	{
		g.title = "test";
		print_game(&g);
		g.win = mlx_new_window(g.mlx, g.wnd_x, g.wnd_y, g.title);
		//*
		g.win_img = mlx_new_image(g.mlx, g.wnd_x, g.wnd_y);
		g.img_addr = mlx_get_data_addr(g.win_img, &g.img_bits_per_pixel, &g.img_line_length, &g.img_endian);

		print_game(&g);

		int i;
		int j;
		i = 0;
		while (i < g.wnd_x)
		{	
			j = 0;
			while (j < g.wnd_y)
			{
				if (j < g.wnd_y / 2)
					my_mlx_pixel_put(&g, i, j, g.ce_rgb);
				else
					my_mlx_pixel_put(&g, i, j, g.fl_rgb);
				j++;
			}
			i++;
		}
		mlx_put_image_to_window(g.mlx, g.win, g.win_img, 0, 0);
		mlx_loop(g.mlx);
	}
	//*/
}
//gcc -I ~/42/minilibx_linux/ main.c ~/42/minilibx_linux/*.a  -Imlx_linux -lXext -lX11 -lm -lz 