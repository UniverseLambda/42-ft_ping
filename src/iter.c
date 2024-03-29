/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iter.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 13:33:34 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/16 14:11:18 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/iter.h"

#include <signal.h>

#include "inc/ft_signal.h"
#include "inc/ft_time.h"
#include "inc/ft_util.h"

void	new_iteration(const t_initedping *ping, t_iter_info *iter)
{
	iter->ping = ping;
	ft_memset(&(iter->resp_origin), 0, sizeof(iter->resp_origin));
	ft_memset(&(iter->ipv4_header), 0, 20);
	ft_memset(&(iter->response_data), 0, 512);
	iter->response_buffer_info[0] = (struct iovec)
	{.iov_base = &(iter->ipv4_header), .iov_len = 20};
	iter->response_buffer_info[1] = (struct iovec)
	{.iov_base = &(iter->response_data), .iov_len = 512};
	iter->is_error = false;
	iter->responded = true;
	iter->msg_header.msg_name = &(iter->resp_origin.sock_addr);
	iter->msg_header.msg_namelen = sizeof(iter->resp_origin.sock_addr);
	iter->msg_header.msg_iov = (iter->response_buffer_info);
	iter->msg_header.msg_iovlen = 2;
}
