/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iter.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 13:33:40 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/07 13:44:26 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITER_H
# define ITER_H

# include "posix_setup.h"

# include <stdint.h>

# include "ft_result.h"

typedef struct s_iter_info
{
	t_sockaddr_res	resp_origin;
	unsigned char	ipv4_header[20];
	unsigned char	response_data[512];
	struct iovec	response_buffer_info[2];
	struct icmphdr	*resp_icmphdr;
	struct msghdr	msg_header;
	uint64_t		sent_instant;
	bool			responded;
	uint64_t		responded_time;
	const char		*error_message;
}	t_iter_info;

void	new_iteration(t_iter_info *iter);
void	new_listen_try(t_iter_info *iter);

#endif // ITER_H
