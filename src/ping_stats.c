/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping_stats.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 15:57:00 by clsaad            #+#    #+#             */
/*   Updated: 2023/05/19 11:45:39 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/ping_stats.h"

#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

#include "inc/ft_time.h"
#include "inc/ft_string.h"
#include "inc/ft_util.h"


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
	handle->rtt_sum += rtt;
	handle->rtt_sq_sum += rtt * rtt;
}

void	pstats_sent(void)
{
	struct s_ping_stats	*handle;

	handle = pstats_handle();
	if (handle->sent == 0)
		handle->start = now_micro();
	handle->sent += 1;
}

struct s_ping_stats pstats_get(void)
{
	return (*pstats_handle());
}
