/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_mapeval.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 10:45:02 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/19 02:25:20 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	free_2d(void **v, size_t i_size)
{
	while (i_size--)
		free(*(v + i_size));
	free(v);
}

static void	set_start_pos(t_god *g, t_ivec *index_v)
{
//	t_fvec g->pdir;
	//t_fvec g->pvew;
	double rot;

	set_ivec(&g->scell, index_v->i, index_v->j);
	if (g->ppos.x > 0)
		set_err_msg(g, "Multi start position error\n");
	set_fvec(&g->ppos, g->scell.i + 0.5, g->scell.j + 0.5);
	set_fvec(&g->pdir, -1, 0);
	set_fvec(&g->pvew, 0, (double)g->wnd.i / (double)g->wnd.j / 2.0);
	rot = 0;
	if (g->map[g->scell.i][g->scell.j] == 'S')
		rot = M_PI;
	else if (g->map[g->scell.i][g->scell.j] == 'E')
		rot = -M_PI_2;
	else if (g->map[g->scell.i][g->scell.j] == 'W')
		rot = M_PI_2;
	rotation(&g->pdir, rot);
	rotation(&g->pvew, rot);
	//g->pvew.x = g->pvew.x;
	//g->pvew.y = g->pvew.y;
}

static int	map_check(t_god *g)
{
	t_ivec	index_v;
	char	**chec_map;

	if (!(chec_map = malloc(sizeof(char *) * (g->map_h))))
		return (set_err_msg(g, "malloc error\n"));
	index_v.i = -1;
	while (++index_v.i < (int)g->map_h)
	{
		if (!(*(chec_map + index_v.i) = ft_strdup(*(g->map + index_v.i))))
		{
			free_2d((void **)chec_map, g->map_h);
			return (set_err_msg(g, "malloc error\n"));
		}
		index_v.j = -1;
		while (++index_v.j < (int)g->map_w)
			if (ft_strchr("NSWE", g->map[index_v.i][index_v.j]))
				set_start_pos(g, &index_v);
	}
	if (g->scell.i < 0 && g->scell.j < 0)
		set_err_msg(g, "No player position\n");
	if (!g->err_msg)
		map_closecheck(g->scell, chec_map, g);
	free_2d((void **)chec_map, g->map_h);
	return (0);
}

static int	make_mapdata(t_god *g)
{
	int		i;
	t_list	*list_p;

	list_p = g->map_list;
	if (!(g->map = malloc(sizeof(char *) * (g->map_h + 1))))
		return (set_err_msg(g, "malloc error\n"));
	*(g->map + g->map_h) = 0;
	i = 0;
	while (i < (int)g->map_h)
	{
		if (!(*(g->map + i) = malloc(sizeof(char) * (g->map_w + 1))))
			return (set_err_msg(g, "malloc error\n"));
		ft_memset(*(g->map + i), ' ', g->map_w);
		ft_memcpy(*(g->map + i), list_p->content, ft_strlen(list_p->content));
		*(*(g->map + i) + g->map_w) = 0;
		list_p = list_p->next;
		i++;
	}
	return (0);
}

void		eval_map(t_god *g, char *line, char *cfg_str, char *no_map_str)
{
	if (!ft_strlen(cfg_str))
	{
		make_mapdata(g);
		map_check(g);
	}
	else if (ft_strlen(no_map_str))
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
