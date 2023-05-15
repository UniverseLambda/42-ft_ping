/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/18 09:18:46 by clsaad            #+#    #+#             */
/*   Updated: 2023/05/15 17:01:40 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/ft_sqrt.h"

uint64_t	ft_sqrt(uint64_t a)
{
	uint64_t	prev_x;
	uint64_t	x;

	prev_x = UINT64_MAX;
	x = a;
	if (a > 0)
	{
		while (x < prev_x)
		{
			prev_x = x;
			x = (prev_x + (a / prev_x)) / 2;
		}
	}
	return (x);
}
