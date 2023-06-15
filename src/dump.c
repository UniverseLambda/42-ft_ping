/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dump.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 15:10:22 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/15 14:52:00 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/dump.h"

#include <stdio.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include "inc/ft_util.h"
#include "inc/iter.h"

inline static uint16_t	ft_ntohs(uint16_t v)
{
	uint16_t	tmp;

	tmp = v & 0xFF00;
	v <<= 8;
	v |= tmp;
	return (v);
}

static void	dump_raw_ip(uint8_t *iphdr)
{
	size_t	index;

	index = 0;
	printf("IP Hdr Dump:\n");
	while (index < sizeof(struct iphdr))
	{
		if (index % 2)
			printf("%.2x", (unsigned int)((iphdr[index])));
		else
			printf(" %.2x", (unsigned int)((iphdr[index])));
		++index;
	}
	printf("\n");
}

static void	dump_ip(struct iphdr *ip)
{
	uint8_t	*cp;
	size_t	header_len;

	cp = (uint8_t *)(ip + 1);
	header_len = ip->ihl;
	printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src\tDst\tData\n");
	printf(" %1x  %1x  %02x", ip->version, ip->ihl, ip->tos);
	if (ip->tot_len > 0x2000)
		printf(" %04x %04x", ft_ntohs(ip->tot_len), ft_ntohs(ip->id));
	else
		printf(" %04x %04x", ip->tot_len, ft_ntohs(ip->id));
	printf("   %1x %04x", ft_ntohs(ip->frag_off) >> 13,
		ft_ntohs(ip->frag_off) & 0x1FFF);
	printf("  %02x  %02x %04x", ip->ttl, ip->protocol, ft_ntohs(ip->check));
	printf(" %u.%u.%u.%u ", (ip->saddr >> 24) & 0xFF, (ip->saddr >> 16) & 0xFF,
		(ip->saddr >> 8) & 0xFF, ip->saddr & 0xFF);
	printf(" %u.%u.%u.%u ", (ip->daddr >> 24) & 0xFF, (ip->daddr >> 16) & 0xFF,
		(ip->daddr >> 8) & 0xFF, ip->daddr & 0xFF);
	while (header_len-- > sizeof (*ip))
		printf("%02x", *cp++);
	printf("\n");
}

void	dump_packet(uint8_t *iphdr, uint8_t *inner_icmp)
{
	const struct icmphdr	*icmp = (struct icmphdr *)inner_icmp;

	dump_raw_ip(iphdr);
	dump_ip((struct iphdr *)iphdr);
	printf("ICMP: type %u code %u, size %u, id 0x%04x, seq 0x%04x\n",
		icmp->type, icmp->code, 8, icmp->un.echo.id, icmp->un.echo.sequence);
}
