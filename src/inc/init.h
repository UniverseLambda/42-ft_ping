/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 10:37:44 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/27 14:59:33 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_H
# define INIT_H

# include <netdb.h>
# include "ft_result.h"
# include "ft_string.h"

typedef struct s_initedping
{
	t_sockaddr_res	sockaddr;
	t_string		address_array[512];
	size_t			address_total;
	t_string		address;
	int				conn_fd;
	bool			verbose;
	uint16_t		icmp_ident;
	uint64_t		*prev_pkt;
	size_t			prev_pkt_len;
}	t_initedping;

t_initedping	ping_init(char **argv);
t_sockaddr_res	select_interface(t_string address);

#endif // INIT_H
