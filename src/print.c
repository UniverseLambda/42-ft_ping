/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 14:36:55 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/13 15:44:27 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/print.h"

#include <stdio.h>

#include "inc/ping_stats.h"
#include "inc/ft_util.h"
#include "inc/ft_sqrt.h"
#include "inc/ft_time.h"
#include "inc/misc.h"

void	print_addr(t_sockaddr_res *sockaddr, char *response_ip)
{
	const char	*addr = resolve_cache_addr(sockaddr);

	if (!addr || !(addr[0]))
		printf("%s", response_ip);
	else
		printf("%s (%s)", addr, response_ip);
}

static char	*time_to_printable(char *buf, uint64_t time)
{
	snprintf(buf, 32, "%lu.%.3lu", time / 1000, time % 1000);
	return (buf);
}

void	print_end_stats(void)
{
	const struct s_ping_stats	stats = pstats_get();
	char						tmp_buf[32];
	uint64_t					mdev;
	uint64_t					average;
	uint64_t					sq_average;

	printf("--- %s ping statistics ---\n", stats.user_addr.data);
	printf("%zu packets transmitted, %zu received, "
		"%zu%% packet loss\n",
		stats.sent,
		stats.received,
		100 - ((stats.received * 100) / stats.sent));
	if (stats.received == 0)
		return ;
	average = stats.rtt_sum / stats.received;
	sq_average = stats.rtt_sq_sum / stats.received;
	mdev = ft_sqrt(sq_average - (average * average));
	printf("round-trip min/avg/max/stddev = %s/",
		time_to_printable(tmp_buf, stats.min));
	printf("%s/", time_to_printable(tmp_buf, average));
	printf("%s/", time_to_printable(tmp_buf, stats.max));
	printf("%s ms\n", time_to_printable(tmp_buf, mdev));
}

void	print_time(uint64_t time)
{
	const uint64_t	left = time / 1000;
	const uint64_t	right = time % 1000;

	printf("%lu.%.3lu ms\n", left, right);
}
