/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 10:37:44 by clsaad            #+#    #+#             */
/*   Updated: 2023/05/19 10:42:01 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_H
# define INIT_H

# define _POSIX_C_SOURCE 200112L
# include <netdb.h>
# include "ft_result.h"
# include "ft_string.h"

typedef struct s_initedping
{
	t_sockaddr_res	sockaddr;
	t_string		address;
	int				conn_fd;
}	t_initedping;

t_initedping ping_init(int argc, char **argv);

#endif // INIT_H
