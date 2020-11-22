/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_mapcheck.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 10:42:45 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/22 21:56:20 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	set_r_on_map(char **map, int i, int j, t_list **lst_last)
{
	t_ivec *tmp;

	if (map[i][j] != '1' && map[i][j] != 'R')
	{
		map[i][j] = 'R';
		if (!(tmp = malloc(sizeof(t_ivec))))
			return (0);
		set_ivec(tmp, i, j);
		(*lst_last)->next = ft_lstnew(tmp);
		*lst_last = (*lst_last)->next;
	}
	return (1);
}

static int	set_r_around(char **map, t_ivec *v, t_list **lst_last)
{
	return (set_r_on_map(map, v->i, v->j - 1, lst_last) &&
			set_r_on_map(map, v->i, v->j + 1, lst_last) &&
			set_r_on_map(map, v->i - 1, v->j, lst_last) &&
			set_r_on_map(map, v->i + 1, v->j, lst_last));
}

static int	check_around(t_god *g, char **map, t_ivec *v)
{
	return (!(v->i != 0 && v->i != (int)g->map_h - 1 &&
		v->j != 0 && v->j != (int)g->map_w - 1 &&
		map[v->i][v->j - 1] != ' ' && map[v->i - 1][v->j] != ' ' &&
		map[v->i][v->j + 1] != ' ' && map[v->i + 1][v->j] != ' '));
}

int			map_closecheck(t_ivec start_pos, char **map, t_god *g)
{
	t_list *lst;
	t_list *lst_last;
	t_ivec *pos;

	if (!(pos = malloc(sizeof(t_ivec))))
		return (set_err_msg(g, "malloc error!"));
	set_ivec(pos, start_pos.i, start_pos.j);
	map[pos->i][pos->j] = 'R';
	lst = ft_lstnew(pos);
	lst_last = lst;
	while (lst && !g->err_msg)
	{
		if (check_around(g, map, (t_ivec *)(lst->content)))
			set_err_msg(g, "map is not closed.\n");
		else if (!set_r_around(map, (t_ivec *)(lst->content), &lst_last))
			set_err_msg(g, "malloc error.\n");
		ft_lstdelhead(&lst, &free);
	}
	ft_lstclear(&lst, &free);
	return (0);
}
