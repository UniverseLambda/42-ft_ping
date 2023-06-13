/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 10:37:44 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/13 15:37:03 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_H
# define INIT_H

# include "posix_setup.h"

# include <netdb.h>
# include "ft_result.h"
# include "ft_string.h"

typedef struct s_initedping
{
	t_sockaddr_res	sockaddr;
	t_string		address;
	int				conn_fd;
	bool			verbose;
	uint16_t		icmp_ident;
}	t_initedping;

t_initedping	ping_init(char **argv);

#endif // INIT_H
