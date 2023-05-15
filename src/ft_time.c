/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_time.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 10:25:06 by clsaad            #+#    #+#             */
/*   Updated: 2023/05/15 10:54:33 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "inc/ft_time.h"

suseconds_t now_micro()
{
	struct timeval	time_tmp;

	// Should not fail, like... I hope *shrug*
	if (0 > gettimeofday(&time_tmp, NULL))
		fprintf(stderr, "ft_ping: gettimeofday error: %s\n", strerror(errno));
	return (time_tmp.tv_usec);
}

