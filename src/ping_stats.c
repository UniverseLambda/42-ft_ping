/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping_stats.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 15:57:00 by clsaad            #+#    #+#             */
/*   Updated: 2023/05/15 17:09:27 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/ping_stats.h"

#include <stdlib.h>
#include <stdint.h>

#include "inc/ft_time.h"
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
	handle->min = UINT64_MAX;
}

void pstats_responded(uint64_t rtt)
{
	struct s_ping_stats	*handle;

	handle = pstats_handle();
	handle->received += 1;
	if (rtt < handle->min)
		handle->min = rtt;
	if (rtt > handle->max)
		handle->max = rtt;
	// FIXME: At icmp_seq == 7, big fuck up here
	handle->average += (rtt - handle->average) / handle->received;
	handle->sq_average += ((rtt * rtt) - handle->sq_average) / handle->received;
}

void	pstats_sent(void)
{
	struct s_ping_stats	*handle;

	handle = pstats_handle();
	if (handle->sent == 0)
		handle->start = now_micro();
	pstats_handle()->sent += 1;
}

struct s_ping_stats pstats_get(void)
{
	return (*pstats_handle());
}
