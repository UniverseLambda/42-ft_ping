/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 16:51:42 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/13 14:42:21 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_error.h"

#include <stdlib.h>
#include <stdio.h>

struct s_icmp_error_type_desc
{
	uint16_t	type;
	const char	*message;
	void		(*code_getter)(uint16_t);
};

static void	type3(uint16_t code);
static void	type5(uint16_t code);
static void	type11(uint16_t code);

static const struct s_icmp_error_type_desc	g_type_desc[] = {
{0, "Echo Reply", NULL},
{3, NULL, type3},
{4, "Source Quench", NULL},
{5, NULL, type5},
{8, "Echo Request", NULL},
{11, NULL, type11},
	// {12, NULL, NULL}, TODO
{13, "Timestamp", NULL},
{14, "Timestamp Reply", NULL},
{15, "Information Request", NULL},
{17, "Address Mask Reply", NULL},
};

static void type3(uint16_t code)
{
	const char	*txt[] = {
		"Destination Net Unreachable",
		"Destination Host Unreachable",
		"Destination Protocol Unreachable",
		"Destination Port Unreachable",
		"Fragmentation needed and DF set",
		"Source Route Failed",
		"Network Unknown",
		"Host Unknown",
		"Host Isolated",
		"Destination Net Prohibited",
		"Destination Host Prohibited",
		"Destination Network Unreachable At This TOS",
		"Destination Host Unreachable At This TOS",
		"Packet Filtered",
		"Precedence Violation",
		"Precedence Cutoff",
	};

	if (code < (sizeof(txt) / sizeof(*txt)))
		printf("%s\n", txt[code]);
	else
		printf("Unknown error\n");
}

static void type5(uint16_t code)
{
	const char	*txt[] = {
		"Redirect Network",
		"Redirect Type of Service and Network",
		"Redirect Host",
		"Redirect Type of Service and Host",
	};

	if (code < (sizeof(txt) / sizeof(*txt)))
		printf("%s\n", txt[code]);
	printf("Unknown error\n");
}

static void	type11(uint16_t code)
{
	if (code == 0)
		printf("Time to live exceeded\n");
	if (code == 1)
		printf("Frag reassembly time exceeded\n");
	printf("Unknown Time Exceeded Message code\n");
}

void get_error(uint16_t type, uint16_t code)
{
	size_t index;

	index = 0;
	while (index < (sizeof(g_type_desc) / sizeof(*g_type_desc)))
	{
		if (g_type_desc[index].type == type)
		{
			if (g_type_desc[index].message)
				printf("%s\n", g_type_desc[index].message);
			if (g_type_desc[index].code_getter)
				g_type_desc[index].code_getter(code);
		}
		++index;
	}
	printf("Bad ICMP type %d", type);
}
