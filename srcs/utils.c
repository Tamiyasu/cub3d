/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 11:19:31 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/15 11:19:42 by tmurakam         ###   ########.fr       */
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

void	set_ivec(t_ivec *v, long i, long j)
{
	v->i = i;
	v->j = j;
}

void	set_fvec(t_fvec *v, double x, double y)
{
	v->x = x;
	v->y = y;
}

void	ft_lstdelhead(t_list **lst, void (*del)(void *))
{
	t_list *lst_tmp;

	lst_tmp = *lst;
	*lst = (*lst)->next;
	ft_lstdelone(lst_tmp, del);
}

void	skip_spacis(char **str)
{
	while (ft_isspace(**str))
		(*str)++;
}
