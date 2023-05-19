/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 10:48:12 by clsaad            #+#    #+#             */
/*   Updated: 2023/05/19 10:49:05 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/ft_signal.h"

int *last_signal(void)
{
	static int	sig = 0;

	return (&sig);
}

void signal_handler(int s)
{
	*last_signal() = s;
}
