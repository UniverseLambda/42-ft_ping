/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dump.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 15:10:22 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/13 16:40:21 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/dump.h"

#include <stdio.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include "inc/ft_util.h"

inline static uint16_t ft_ntohs(uint16_t v)
{
	uint16_t	tmp;

	tmp = v & 0xFF00;
	v <<= 8;
	v |= tmp;

	return (v);
}

static void dump_raw_ip(char *iphdr)
{
	size_t	index;

	index = 0;
	printf("IP Hdr Dump:\n");
	while (index < sizeof(struct iphdr))
	{
		if (index % 2)
			printf("%.2x", iphdr[index]);
		else
			printf(" %.2x", iphdr[index]);
	}
	printf("\n");
}

static void dump_ip(struct iphdr *ip)
{
	unsigned char	*cp;
	size_t	header_len;

	cp = (unsigned char *)(ip + 1);
	header_len = ip->ihl;
	printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src\tDst\tData\n");
	printf(" %1x  %1x  %02x", ip->version, ip->ihl, ip->tos);
	if (ip->tot_len > 0x2000)
		printf(" %04x %04x", ft_ntohs(ip->tot_len), ft_ntohs(ip->id));
	else
		printf(" %04x %04x", ip->tot_len, ft_ntohs(ip->id));

	printf("   %1x %04x", ft_ntohs(ip->frag_off) >> 13, ft_ntohs(ip->frag_off) & 0x1FFF);
	printf("  %02x  %02x %04x", ip->ttl, ip->protocol, ft_ntohs(ip->check));
	printf(" %u.%u.%u.%u ",
		ip->saddr & 0xFF, (ip->saddr >> 8) & 0xFF, (ip->saddr >> 16) & 0xFF, (ip->saddr >> 24) & 0xFF);
	printf(" %u.%u.%u.%u ",
		ip->daddr & 0xFF, (ip->daddr >> 8) & 0xFF, (ip->daddr >> 16) & 0xFF, (ip->daddr >> 24) & 0xFF);
	while (header_len-- > sizeof (*ip))
    	printf("%02x", *cp++);
	printf("\n");
}

void dump_icmp(struct icmphdr *icmp)
{
	printf("ICMP: type %u code %u, size %u, id 0x%04x, seq 0x%04x\n",
		icmp->type, icmp->code, icmp->un.echo.id, icmp->un.echo.sequence);
}

void dump_packet(char *iphdr, char *inner_icmp)
{
	// TODO: ADD VERBOSITY CHECK
	// TODO: CALL THIS BAD MOTHER FUCKER
	// if (verbose)
		dump_raw_ip(iphdr);
	dump_ip((struct iphdr *)iphdr);
	dump_icmp((struct icmphdr *)inner_icmp);
}
