/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:17:50 by clsaad            #+#    #+#             */
/*   Updated: 2023/05/15 17:06:01 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200112L

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "inc/cli.h"
#include "inc/ft_result.h"
#include "inc/ft_sqrt.h"
#include "inc/ft_string.h"
#include "inc/ft_time.h"
#include "inc/ft_util.h"
#include "inc/ping_stats.h"


int usleep(useconds_t usec);

static void make_icmp_packet(struct icmphdr *icmp_header, t_string payload, char *dest)
{
	ft_memcpy(dest, icmp_header, sizeof(*icmp_header));
	ft_memcpy(dest + sizeof(*icmp_header), payload.data, payload.len);
}

static struct addrinfo *resolve_host(t_string host)
{
	struct addrinfo hints = {0};
	struct addrinfo *result;

	hints.ai_family = AF_INET;			/* Allow IPv4 only */
	hints.ai_socktype = SOCK_RAW;		/* Raw socket */
	hints.ai_protocol = IPPROTO_ICMP;	/* ICMP protocol */
	// printf("Resolving \"%s\"\n", host.data);
	int gai_ret_val = getaddrinfo(host.data, NULL, &hints, &result);

	if (gai_ret_val != 0)
	{
		fprintf(stderr, "ft_ping: %s: %s\n", host.data, gai_strerror(gai_ret_val));
		exit(1);
	}

	return result;
}

static int *last_signal(void)
{
	static int	sig = 0;

	return (&sig);
}

static void signal_handler(int s)
{
	*last_signal() = s;
}

uint16_t compute_checksum(void *data, size_t data_len)
{
	size_t word_len = data_len / 2;
	uint32_t sum = 0xFFFF;

	for (size_t i = 0; i < word_len; ++i)
	{
		sum += ((uint16_t *)data)[i];

		if (sum > 0xFFFF)
		{
			sum -= 0XFFFF;
		}
	}

	return sum;
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

static void	print_time(uint64_t time)
{
	const uint64_t	left	= time / 1000;
	const uint64_t	right	= time % 1000;
	char buffer[7];

	if (left >= 100)
		printf("%lums\n", left);
	else
	{
		ft_memset(buffer, 0, 6);
		snprintf(buffer, 7, "%lu.%.3lu", left, right);
		printf("%.*sms\n", 5 - (left != 0), buffer);
	}
}

static t_result send_icmp_echo(int conn_fd, int *sequence, t_sockaddr_res sa)
{
	struct icmphdr	header = {0};
	char			buffer[512];
	const t_string	data = string_new("Lorem ipsum dolor sit amet eleifend.");

	header.type = ICMP_ECHO;
	header.un.echo.id = getpid();
	header.un.echo.sequence = ++(*sequence);
	make_icmp_packet(&header, data, buffer);
	// Properly setting checksum without re-copying the whole packet into the buffer
	((uint16_t *)buffer)[1] = ~compute_checksum(buffer, sizeof(header) + data.len);
	pstats_sent();
	if (0 > sendto(conn_fd, buffer, sizeof(header) + data.len, 0, &sa.sock_addr, sa.sock_addr_len))
		return (result_err(errno));
	return (result_ok((union u_resultable)0));
}

static char *time_to_printable(char *buf, uint64_t time) {
	if (time == 0)
		return "0";
	snprintf(buf, 32, "%lu.%lu", time / 1000, time % 1000);

	return (buf);
}

static void print_end_stats(void)
{
	const struct s_ping_stats	stats = pstats_get();
	char						tmp_buf[32];
	uint64_t					mdev;

	printf("\n--- %s ping statistics ---\n", stats.user_addr.data);
	printf("%zu packets transmitted, %zu received, %zu%% packet loss, time %ldms\n",
		stats.sent,
		stats.received,
		100 - ((stats.received * 100) / stats.sent),
		(now_micro() - stats.start) / 1000);

	if (stats.received == 0)
		return;
	mdev = ft_sqrt(stats.sq_average - (stats.average * stats.average));
	printf("rtt min/avg/max/mdev = %s/", time_to_printable(tmp_buf, stats.min));
	printf("%s/", time_to_printable(tmp_buf, stats.average));
	printf("%s/", time_to_printable(tmp_buf, stats.max));
	printf("%s ms\n", time_to_printable(tmp_buf, mdev));
}

int main(int argc, char **argv)
{
	t_command		cmd;
	t_sockaddr_res	sockaddr;

	cmd = ftp_command(argc, argv);

	int conn_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (conn_fd == -1)
	{
		// FIXME: check for errno usage for subject compliance
		fprintf(stderr, "ft_ping: %s: %s\n", cmd.address.data, strerror(errno));
		exit(2);
	}

	sockaddr = select_interface(cmd.address);

	int sequence = 0;

	printf("PING %s 56(84) bytes\n", cmd.address.data);

	pstats_init(cmd.address);

	char response_origin[255];
	unsigned char ipv4_header[20];
	unsigned char response_data[512];
	signal(SIGINT, signal_handler);

	while (1)
	{
		uint64_t sent_instant;
		struct msghdr msg_header = {0};

		signal(SIGALRM, signal_handler);
		ft_memset(response_origin, 0, sizeof(response_origin));
		ft_memset(ipv4_header, 0, sizeof(ipv4_header));
		ft_memset(response_data, 0, sizeof(response_data));

		struct iovec response_buffer_info[2] = {
			(struct iovec)
			{ .iov_base = ipv4_header, .iov_len = sizeof(ipv4_header)},
			(struct iovec)
			{ .iov_base = response_data, .iov_len = sizeof(response_data)},
		};


		t_result send_res = send_icmp_echo(conn_fd, &sequence, sockaddr);

		if (result_is_err(&send_res))
		{
			fprintf(stderr, "ft_ping: %s: %s\n", cmd.address.data, strerror(send_res.payload.error_code));
			exit(2);
		}

		sent_instant = now_micro();

		uint64_t response_time = 0;
		struct icmphdr *response_icmphdr;
		bool responded = true;
		// unsigned char *response_payload;

		alarm(1);
		while (1)
		{
			msg_header.msg_name = response_origin;
			msg_header.msg_namelen = sizeof(response_origin);
			msg_header.msg_iov = response_buffer_info;
			msg_header.msg_iovlen = 2;

			ssize_t read = recvmsg(conn_fd, &msg_header, MSG_WAITALL);
			uint64_t response_instant = now_micro();

			response_time = response_instant - sent_instant;

			if (read <= 0)
			{
				if (errno == EINTR)
				{
					responded = false;
					break;
				}

				fprintf(stderr, "ft_ping: %s: %s\n", cmd.address.data, strerror(errno));
				exit(2);
			}

			size_t icmphdr_offset = (((ipv4_header[0]) & 0x0F) - 5) * 4;
			// size_t icmp_data_offset = icmphdr_offset + sizeof(struct icmphdr);

			response_icmphdr = (struct icmphdr *)(response_data + (icmphdr_offset));
			// response_payload = response_data + icmp_data_offset;

			if (response_icmphdr->un.echo.id == getpid() && response_icmphdr->type == 0 && response_icmphdr->un.echo.sequence == sequence)
				break;
		}

		alarm(0);

		if (*last_signal() != SIGINT && response_time < 1000000) {
			usleep(1000000 - response_time);
		}

		if (*last_signal() == SIGINT) {
			break;
		}

		if (responded)
		{
			char response_ip[INET_ADDRSTRLEN] = {0};

			pstats_responded(response_time);

			inet_ntop(AF_INET, &((struct sockaddr_in *)&sockaddr.sock_addr)->sin_addr, response_ip, INET_ADDRSTRLEN);

			uint16_t packet_len = ((uint16_t)ipv4_header[2] << 8) | ipv4_header[3];
			printf("%u bytes from %s: icmp_seq=%u ttl=%u time=", packet_len, response_ip, response_icmphdr->un.echo.sequence, ipv4_header[8]);

			print_time(response_time);
		}

	}

	print_end_stats();

	return (0);
}
