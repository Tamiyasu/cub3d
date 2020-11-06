/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 20:44:50 by tmurakam          #+#    #+#             */
/*   Updated: 2020/10/13 20:49:09 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int ft_strcmp(const char *s1, const char *s2)
{
    while(*s1 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}