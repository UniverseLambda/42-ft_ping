/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:17:50 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/16 22:13:14 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/posix_setup.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

#include "inc/cli.h"
#include "inc/ft_result.h"
#include "inc/ft_signal.h"
#include "inc/ft_sqrt.h"
#include "inc/ft_string.h"
#include "inc/ft_time.h"
#include "inc/ft_util.h"
#include "inc/get_error.h"
#include "inc/init.h"
#include "inc/ping_stats.h"
#include "inc/iter.h"
#include "inc/packet_util.h"
#include "inc/print.h"
#include "inc/misc.h"
#include "inc/ft_bits.h"

int	usleep(useconds_t usec);

static t_result	send_icmp_echo(
	int conn_fd, uint16_t *sequence, t_sockaddr_res sa)
{
	t_icmphdr		header;
	char			buffer[512];
	const t_string	data = string_new(
			"        Lorem ipsum dolor sit amet, consectetur posuere.");

	header = (t_icmphdr){0};
	header.type = ICMP_ECHO;
	header.un.echo.id = getpid();
	header.un.echo.sequence = ++(*sequence);
	make_icmp_packet(&header, data, buffer);
	((uint16_t *)buffer)[1]
		= ~compute_checksum(buffer, sizeof(header) + data.len);
	if (0 > sendto(conn_fd, buffer, sizeof(header) + data.len, 0,
			&sa.sock_addr, sa.sock_addr_len) || *last_signal() == SIGINT)
		return (result_err(errno));
	pstats_sent();
	return (result_ok((union u_resultable)0));
}

static bool	try_send_message(const t_initedping *ping, uint16_t *sequence)
{
	t_result	send_res;

	send_res = send_icmp_echo(ping->conn_fd, sequence, ping->sockaddr);
	if (result_is_err(&send_res))
	{
		if (send_res.payload.error_code == EINTR)
			return (false);
		fprintf(stderr, "ft_ping: %s: %s\n",
			ping->address.data, strerror(send_res.payload.error_code));
		exit(2);
	}
	unset_bit(ping->prev_pkt, ping->prev_pkt_len,
		*sequence);
	return (true);
}

static bool	try_listen_for_answer(
	const t_initedping *ping, t_iter_info *iter)
{
	size_t	icmp_off;

	if (recvmsg(ping->conn_fd, &iter->msg_header, MSG_WAITALL) <= 0)
	{
		if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
		{
			iter->responded = false;
			return (true);
		}
		fprintf(stderr,
			"ft_ping: %s: %s\n", ping->address.data, strerror(errno));
		exit(2);
	}
	icmp_off = get_icmphdr_offset((char *)iter->ipv4_header) - (4 * 5);
	iter->resp_icmphdr = (t_icmphdr *)(iter->response_data + (icmp_off));
	iter->responded_time = now_micro() - micro_from_timestamp(
			((uint64_t *)iter->resp_icmphdr)[1]);
	iter->resp_origin.sock_addr_len = iter->msg_header.msg_namelen;
	return (handle_packet(iter));
}

static void	start_ping(const t_initedping *ping)
{
	uint16_t	sequence;
	t_iter_info	iter;
	uint64_t	last_sent;

	sequence = 0;
	while (1)
	{
		if (!try_send_message(ping, &sequence))
			break ;
		last_sent = now_micro();
		while (now_micro() - last_sent < 1000000)
		{
			new_iteration(ping, &iter);
			if (*last_signal() == SIGINT)
				return ;
			if (!try_listen_for_answer(ping, &iter))
				continue ;
			else if (iter.responded)
			{
				received_stats(&iter, sequence);
				set_bit(ping->prev_pkt, ping->prev_pkt_len, sequence);
			}
		}
	}
}

int	main(int argc, char **argv)
{
	const t_initedping	ping = ping_init(argv);

	(void)argc;
	printf("PING %s: 56 data bytes", ping.address.data);
	if (ping.verbose)
		printf(", id 0x%1$04x = %1$u", ping.icmp_ident);
	printf("\n");
	start_ping(&ping);
	print_end_stats();
	return (0);
}
