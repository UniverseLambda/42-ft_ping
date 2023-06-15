/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_time.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 10:25:06 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/15 16:29:23 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/ft_time.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>

uint64_t	now_micro(void)
{
	struct timeval	time_tmp;

	if (0 > gettimeofday(&time_tmp, NULL))
		fprintf(stderr, "ft_ping: gettimeofday error: %s\n", strerror(errno));
	return (time_tmp.tv_sec * 1000000ull + time_tmp.tv_usec);
}

uint64_t	now_sec(void)
{
	struct timeval	time_tmp;

	if (0 > gettimeofday(&time_tmp, NULL))
		fprintf(stderr, "ft_ping: gettimeofday error: %s\n", strerror(errno));
	return (time_tmp.tv_sec);
}

uint64_t	now_timestamp(void)
{
	struct timeval	time_tmp;

	if (0 > gettimeofday(&time_tmp, NULL))
		fprintf(stderr, "ft_ping: gettimeofday error: %s\n", strerror(errno));
	return (((uint64_t)time_tmp.tv_sec) | ((uint64_t)time_tmp.tv_usec << 32));
}

uint64_t	micro_from_timestamp(uint64_t timestamp)
{
	const uint64_t	sec = timestamp & 0xFFFFFFFF;
	const uint64_t	usec = (timestamp >> 32) & 0xFFFFFFFF;

	return (sec * 1000000ull + usec);
}
