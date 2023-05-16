/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping_stats.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 15:56:51 by clsaad            #+#    #+#             */
/*   Updated: 2023/05/16 10:11:20 by clsaad           ###   ########.fr       */
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
	uint64_t	start;
	uint64_t	min;
	uint64_t	rtt_sum;
	uint64_t	rtt_sq_sum;
	uint64_t	max;
	size_t		sent;
	size_t		received;
	t_string	user_addr;
};

void	pstats_init(t_string user_addr);
void	pstats_responded(uint64_t rtt);
void	pstats_sent(void);
struct s_ping_stats	pstats_get(void);

#endif // PING_STATS_H
