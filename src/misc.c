/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 17:11:02 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/08 12:55:54 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/posix_setup.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>

#include "inc/ft_result.h"
#include "inc/ft_util.h"
#include "inc/iter.h"
#include "inc/print.h"
#include "inc/ping_stats.h"
#include "inc/packet_util.h"
#include "inc/get_error.h"

char	*resolve_cache_addr(const t_sockaddr_res *sockaddr)
{
	static t_sockaddr_res	last_sockaddr;
	static char				buf[257];

	if (ft_memcmp(&sockaddr->sock_addr, &last_sockaddr, sockaddr->sock_addr_len
			+ sizeof(sockaddr->sock_addr_len)) == 0)
		return (buf);
	last_sockaddr = *sockaddr;
	ft_memset(buf, 0, sizeof(buf));
	if (getnameinfo(&last_sockaddr.sock_addr, sizeof(last_sockaddr.sock_addr),
			buf, sizeof(buf), NULL, 0, NI_NAMEREQD))
		buf[0] = '\0';
	return (buf);
}

void	received_stats(t_iter_info *iter, uint16_t sequence)
{
	char		response_ip[INET_ADDRSTRLEN];
	uint16_t	packet_len;

	if (!iter->error_message)
		pstats_responded(iter->responded_time);
	inet_ntop(AF_INET, &(sockaddr_in(&iter->resp_origin.sock_addr))->sin_addr,
		response_ip, INET_ADDRSTRLEN);
	if (iter->error_message)
	{
		printf("From ");
		print_addr(&iter->resp_origin, response_ip);
		printf(" icmp_seq=%u %s\n", sequence, iter->error_message);
	}
	else
	{
		packet_len = (((uint16_t)iter->ipv4_header[2] << 8)
				| iter->ipv4_header[3]) - ((iter->ipv4_header[0] & 0x0F) * 4);
		printf("%u bytes from ", packet_len);
		print_addr(&iter->resp_origin, response_ip);
		printf(": icmp_seq=%u ttl=%u time=",
			iter->resp_icmphdr->un.echo.sequence, iter->ipv4_header[8]);
		print_time(iter->responded_time);
	}
}

bool	handle_packet(t_iter_info *iter, uint16_t sequence)
{
	if (!is_ours((char *)(iter->resp_icmphdr), sequence))
		return (false);
	if (iter->resp_icmphdr->type == 0)
	{
		if (iter->resp_icmphdr->un.echo.id == getpid()
			&& iter->resp_icmphdr->un.echo.sequence == sequence)
			return (true);
	}
	else if (iter->resp_icmphdr->type != 8)
	{
		iter->error_message
			= get_error(iter->resp_icmphdr->type, iter->resp_icmphdr->code);
		return (true);
	}
	return (false);
}
