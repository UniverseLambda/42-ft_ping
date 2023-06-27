/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 17:11:02 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/27 14:59:48 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
#include "inc/dump.h"
#include "inc/ft_bits.h"

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
			buf, sizeof(buf), NULL, 0, 0))
		buf[0] = '\0';
	return (buf);
}

static inline const char	*get_dup_str(t_iter_info *iter, uint16_t pkt_seq)
{
	if (is_bit_set(iter->ping->prev_pkt, iter->ping->prev_pkt_len, pkt_seq))
		return (" (DUP!)");
	return ("");
}

void	received_stats(t_iter_info *iter)
{
	char		response_ip[INET_ADDRSTRLEN];
	uint16_t	packet_len;

	if (!iter->is_error)
		pstats_responded(iter->responded_time);
	inet_ntop(AF_INET, &(sockaddr_in(&iter->resp_origin.sock_addr))->sin_addr,
		response_ip, INET_ADDRSTRLEN);
	if (!iter->is_error)
	{
		packet_len = (((uint16_t)iter->ipv4_header[2] << 8)
				| iter->ipv4_header[3]) - ((iter->ipv4_header[0] & 0x0F) * 4);
		printf("%u bytes from %s: icmp_seq=%u ttl=%u time=", packet_len,
			response_ip, iter->resp_icmphdr->un.echo.sequence,
			iter->ipv4_header[8]);
		print_time(iter->responded_time);
		printf("%s\n", get_dup_str(iter, iter->resp_icmphdr->un.echo.sequence));
	}
}

bool	handle_packet(t_iter_info *iter)
{
	const uint16_t	type = iter->resp_icmphdr->type;
	uint8_t			*subhdr;
	char			response_ip[INET_ADDRSTRLEN];
	uint16_t		packet_len;

	if (type == 8 || type == 13 || type == 17)
		return (false);
	if (!is_ours((char *)(iter->resp_icmphdr), false))
		return (false);
	if (type == 0)
		return (true);
	iter->is_error = true;
	inet_ntop(AF_INET, &(sockaddr_in(&iter->resp_origin.sock_addr))->sin_addr,
		response_ip, INET_ADDRSTRLEN);
	packet_len = iter->recv_len - ((iter->ipv4_header[0] & 0x0F) * 4);
	printf("%u bytes from %s: ", packet_len, response_ip);
	get_error(type, iter->resp_icmphdr->code);
	if (!iter->ping->verbose)
		return (true);
	subhdr = ((uint8_t *)iter->resp_icmphdr) + 8;
	subhdr += get_icmphdr_offset(subhdr);
	dump_packet(iter->ipv4_header, subhdr);
	return (true);
}
