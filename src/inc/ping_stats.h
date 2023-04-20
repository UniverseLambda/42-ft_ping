/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping_stats.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 15:56:51 by clsaad            #+#    #+#             */
/*   Updated: 2023/04/20 15:14:51 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PING_STATS_H
# define PING_STATS_H

# include <sys/time.h>
# include <stdlib.h>
# include <stdint.h>
# include "ft_string.h"

struct s_ping_stats
{
	suseconds_t	start;
	suseconds_t	min;
	suseconds_t	average;
	suseconds_t	max;
	suseconds_t	mdev;
	size_t		sent;
	size_t		received;
	t_string	user_addr;
};

void pstats_init(t_string user_addr);
void pstats_responded(suseconds_t rtt);
suseconds_t	pstats_sent(void);
struct s_ping_stats pstats_get(void);

#endif // PING_STATS_H
