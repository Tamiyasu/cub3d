/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_initialize.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 11:08:34 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/19 01:15:53 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	check_settings(t_god *g)
{
	if (g->ce_rgb < 0 || g->fl_rgb < 0)
		set_err_msg(g, "Please set color for ceiling / floor\n");
	if (!g->wnd.i || !g->wnd.j)
		set_err_msg(g, "Please set screen size.\n");
	if (!g->ea_img.p)
		set_err_msg(g, "Please set east wall texture.\n");
	if (!g->we_img.p)
		set_err_msg(g, "Please set west  wall texture.\n");
	if (!g->so_img.p)
		set_err_msg(g, "Please set south wall texture.\n");
	if (!g->no_img.p)
		set_err_msg(g, "Please set north wall texture.\n");
	if (!g->s_img.p)
		set_err_msg(g, "Please set sprite wall texture.\n");
	return (0);
}

static void	eval_conf(t_god *g, char *line, char *cfg_str, char *no_map_str)
{
	if (ft_strlen(cfg_str) && !ft_memcmp(cfg_str, "R ", 2))
		read_r(g, cfg_str);
	else if (ft_strlen(cfg_str) &&
	(!ft_memcmp(cfg_str, "C ", 2) || !ft_memcmp(cfg_str, "F ", 2)))
		read_color(g, cfg_str);
	else if (ft_strlen(cfg_str) && (!ft_memcmp(cfg_str, "SO ", 3) ||
							!ft_memcmp(cfg_str, "NO ", 3) ||
							!ft_memcmp(cfg_str, "WE ", 3) ||
							!ft_memcmp(cfg_str, "EA ", 3) ||
							!ft_memcmp(cfg_str, "S ", 2)))
		read_img(g, cfg_str);
	else if (ft_strlen(cfg_str) && !ft_strlen(no_map_str))
	{
		ft_lstadd_back(&g->map_list, ft_lstnew(ft_strdup(line)));
		g->map_h += 1;
		g->map_w = ft_strlen(line);
	}
	else if (ft_strlen(cfg_str))
		set_err_msg(g, "Unknown setting.\n");
}

static void	interpret_line(t_god *g, char *line)
{
	char	*temp_str;
	char	*non_map_parts_tmp;

	if (!(temp_str = ft_strtrim(line, " \t\n\v\f\r")))
		set_err_msg(g, "malloc error!\n");
	if (!(non_map_parts_tmp = ft_strtrim(line, " 012NSWE")))
		set_err_msg(g, "malloc error!\n");
	if (!g->map && !g->map_list && temp_str && non_map_parts_tmp)
		eval_conf(g, line, temp_str, non_map_parts_tmp);
	else if (!g->map && temp_str && non_map_parts_tmp)
		eval_map(g, line, temp_str, non_map_parts_tmp);
	else if (ft_strlen(temp_str))
		set_err_msg(g, "Don't put any describing after map\n");
	free(temp_str);
	free(non_map_parts_tmp);
	free(line);
}

int			load_settings(t_god *g, int argc, char **argv)
{
	int		cub_f_len;
	int		fd;
	char	*line;

	if (argc < 2 || 3 < argc)
		return (set_err_msg(g, "Please specify the argument correctly.\n"));
	cub_f_len = ft_strlen(argv[1]);
	if (cub_f_len <= 4 || ft_strncmp(argv[1] + cub_f_len - 4, ".cub", 4))
		return (set_err_msg(g, "Please set '.cub' file.\n"));
	if (argc == 3 && ft_strncmp(argv[2], "--save", ft_strlen(argv[2])))
		return (set_err_msg(g, "plsese use \"--save\" in 2nd args\n"));
	g->cub_fname = ft_strdup(argv[1]);
	if (!g->cub_fname || 0 > (fd = open(g->cub_fname, O_RDONLY)))
		return (set_err_msg(g, "the '.cub' file is not exist!\n"));
	while (0 < get_next_line(fd, &line))
		interpret_line(g, line);
	interpret_line(g, line);
	line = ft_strdup("");
	interpret_line(g, line);
	g->bmp = argc == 3 ? 1 : 0;
	mlx_do_key_autorepeatoff(g->mlx);
	check_settings(g);
	if(!g->err_msg)
		g->w_img.p = mlx_new_image(g->mlx, g->wnd.i, g->wnd.j);
	if(!g->err_msg)
		g->w_img.addr = mlx_get_data_addr(
			g->w_img.p, &g->w_img.bpp, &g->w_img.llen, &g->w_img.endian);
	return (0);
}
