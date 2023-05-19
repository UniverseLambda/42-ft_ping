/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 10:32:01 by clsaad            #+#    #+#             */
/*   Updated: 2023/05/19 10:51:19 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/init.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "inc/cli.h"
#include "inc/ft_string.h"
#include "inc/ft_result.h"
#include "inc/ping_stats.h"
#include "inc/ft_signal.h"

static struct addrinfo *resolve_host(t_string host)
{
	struct addrinfo hints = {0};
	struct addrinfo *result;

	hints.ai_family = AF_INET;			/* Allow IPv4 only */
	hints.ai_socktype = SOCK_RAW;		/* Raw socket */
	hints.ai_protocol = IPPROTO_ICMP;	/* ICMP protocol */
	int gai_ret_val = getaddrinfo(host.data, NULL, &hints, &result);
	if (gai_ret_val != 0)
	{
		fprintf(stderr, "ft_ping: %s: %s\n", host.data, gai_strerror(gai_ret_val));
		exit(1);
	}

	return result;
}

t_sockaddr_res	select_interface(t_string address)
{
	struct addrinfo	*current;
	struct addrinfo	*resolved;
	struct sockaddr selected_address = {0};
	size_t selected_addresslen = 0;

	resolved = resolve_host(address);
	current = resolved;
	while (current != NULL)
	{
		if (current->ai_addr && current->ai_addrlen > 0)
		{
			selected_address = *(current->ai_addr);
			selected_addresslen = current->ai_addrlen;
			break;
		}
		current = current->ai_next;
	}
	freeaddrinfo(resolved);
	return ((t_sockaddr_res){ .sock_addr = selected_address, .sock_addr_len = selected_addresslen });
}


t_initedping ping_init(int argc, char **argv)
{
	t_command		cmd;
	t_initedping	res;

	cmd = ftp_command(argc, argv);
	res.conn_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	cmd = ftp_command(argc, argv);
	if (res.conn_fd == -1)
	{
		fprintf(stderr, "ft_ping: %s: %s\n", cmd.address.data, strerror(errno));
		exit(2);
	}
	res.sockaddr = select_interface(cmd.address);
	res.address = cmd.address;
	pstats_init(cmd.address);
	signal(SIGINT, signal_handler);
	return (res);
}
