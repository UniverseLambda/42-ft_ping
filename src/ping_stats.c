/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping_stats.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 15:57:00 by clsaad            #+#    #+#             */
/*   Updated: 2023/04/20 15:14:55 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/ping_stats.h"

#include <stdlib.h>
#include <stdint.h>

#include "inc/ft_string.h"
#include "inc/ft_util.h"

#include <sys/time.h>

static struct s_ping_stats *pstats_handle(void)
{
	static struct s_ping_stats	stats;

	return (&stats);
}

void pstats_init(t_string user_addr)
{
	struct s_ping_stats	*handle;

	handle = pstats_handle();
	ft_memset(handle, 0, sizeof(struct s_ping_stats));
	handle->user_addr = user_addr;
	handle->min = __LONG_MAX__;
}

void pstats_responded(suseconds_t rtt)
{
	struct s_ping_stats	*handle;

	handle = pstats_handle();
	handle->received += 1;
	if (rtt < handle->min)
		handle->min = rtt;
	if (rtt > handle->max)
		handle->max = rtt;
	handle->average += (rtt - handle->average) / handle->received;
}

suseconds_t	pstats_sent(void)
{
	struct s_ping_stats	*handle;
	struct timeval		time_tmp;

	handle = pstats_handle();
	gettimeofday(&time_tmp, NULL);
	if (handle->sent == 0)
		handle->start = time_tmp.tv_usec / 1000;
	pstats_handle()->sent += 1;
	return (time_tmp.tv_usec / 1000);
}

struct s_ping_stats pstats_get(void)
{
	return (*pstats_handle());
}
