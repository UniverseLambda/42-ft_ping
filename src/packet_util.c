/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 13:39:29 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/27 11:51:52 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/packet_util.h"
#include "inc/ft_string.h"
#include "inc/ft_time.h"
#include "inc/ft_util.h"

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

size_t	get_icmphdr_offset(void *ipv4_header)
{
	return ((((char *)ipv4_header)[0] & 0x0F) * 4);
}

bool	is_ours(char *icmp_buf, bool recursive)
{
	const t_icmphdr	*icmp_hdr = (t_icmphdr *)icmp_buf;
	char			*sub_hdr;

	if (recursive)
		return (icmp_hdr->type == 8
			&& icmp_hdr->un.echo.id == getpid());
	if (icmp_hdr->type == 0)
		return (icmp_hdr->un.echo.id == getpid());
	else if (icmp_hdr->type < 13)
	{
		sub_hdr = icmp_buf + 8;
		if (((struct iphdr *)sub_hdr)->protocol != IPPROTO_ICMP)
			return (false);
		sub_hdr += get_icmphdr_offset(sub_hdr);
		return (is_ours(sub_hdr, true));
	}
	return (false);
}

void	make_icmp_packet(
	t_icmphdr *icmp_header, t_string payload, char *dest)
{
	const uint64_t	timestamp = now_timestamp();

	ft_memcpy(dest, icmp_header, sizeof(*icmp_header));
	ft_memcpy(dest + sizeof(*icmp_header), payload.data, payload.len);
	ft_memcpy(dest + sizeof(*icmp_header), &timestamp, sizeof(timestamp));
}

uint16_t	compute_checksum(void *data, size_t data_len)
{
	size_t		word_len;
	uint32_t	sum;
	uint32_t	i;

	word_len = data_len / 2;
	sum = 0xFFFF;
	i = 0;
	while (i < word_len)
	{
		sum += ((uint16_t *)data)[i];
		if (sum > 0xFFFF)
		{
			sum -= 0XFFFF;
		}
		++i;
	}
	return (sum);
}

const struct sockaddr_in	*sockaddr_in(const void *ptr)
{
	return (ptr);
}
