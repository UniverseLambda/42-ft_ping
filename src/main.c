/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:17:50 by clsaad            #+#    #+#             */
/*   Updated: 2023/03/27 17:57:40 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200112L

#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "inc/ft_result.h"
#include "inc/ft_string.h"
#include "inc/ft_util.h"
#include "inc/cli.h"


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
	hints.ai_socktype = SOCK_RAW;		/* Datagram socket */
	hints.ai_protocol = IPPROTO_ICMP;	/* ICMP protocol */
	printf("Resolving \"%s\"\n", host.data);
	int gai_ret_val = getaddrinfo(host.data, NULL, &hints, &result);

	if (gai_ret_val != 0)
	{
		fprintf(stderr, "ft_ping: %s: %s\n", host.data, gai_strerror(gai_ret_val));
		exit(1);
	}

	return result;
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

int main(int argc, char **argv)
{
	t_command	cmd;

	cmd = ftp_command(argc, argv);

	struct addrinfo *current;
	struct addrinfo *resolved = resolve_host(cmd.address);
	struct sockaddr selected_address = {0};
	size_t selected_addresslen = 0;
	int conn_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	int sequence = 0;

	for (current = resolved; current != NULL; current = current->ai_next)
	{
		if (conn_fd == -1)
		{
			// FIXME: check for errno usage for subject compliance
			fprintf(stderr, "ft_ping: %s: %s\n", cmd.address.data, strerror(errno));
			exit(2);
		}

		selected_address = *(current->ai_addr);
		selected_addresslen = current->ai_addrlen;

		break;
	}

	freeaddrinfo(resolved);

	if (conn_fd == -1)
		fprintf(stderr, "ft_ping: %s: Host not found\n", cmd.address.data);

	printf("PING %s 56(84) bytes\n", cmd.address.data);

	struct icmphdr header = {0};
	header.type = ICMP_ECHO;
	header.un.echo.id = getpid();

	char buffer[1024];

	t_string data = string_new("MESSAGE!");

	char response_origin[255];
	unsigned char ipv4_header[20];
	unsigned char response_data[1024];

	struct timeval time_tmp;

	while (1)
	{
		suseconds_t sent_instant;
		struct msghdr msg_header = {0};

		ft_memset(response_origin, 0, sizeof(response_origin));
		ft_memset(ipv4_header, 0, sizeof(ipv4_header));
		ft_memset(response_data, 0, sizeof(response_data));


		struct iovec response_buffer_info[2] = {
			(struct iovec)
			{ .iov_base = ipv4_header, .iov_len = sizeof(ipv4_header)},
			(struct iovec)
			{ .iov_base = response_data, .iov_len = sizeof(response_data)},
		};

		header.un.echo.sequence = ++sequence;

		make_icmp_packet(&header, data, buffer);

		uint16_t cs = ~compute_checksum(buffer, sizeof(header) + data.len);

		// Properly setting checksum without re-copying the whole packet into the buffer
		((uint16_t *)buffer)[1] = cs;

		int written = sendto(conn_fd, buffer, sizeof(header) + data.len, 0, &selected_address, selected_addresslen);

		// Should not fail, like... I hope *shrug*
		gettimeofday(&time_tmp, NULL);
		sent_instant = time_tmp.tv_usec;

		if (written <= 0)
		{
			fprintf(stderr, "ft_ping: %s: %s\n", cmd.address.data, strerror(errno));
			exit(2);
		}

		suseconds_t response_time = 0;
		struct icmphdr *response_icmphdr;
		unsigned char *response_payload;

		while (1)
		{
			msg_header.msg_name = response_origin;
			msg_header.msg_namelen = sizeof(response_origin);
			msg_header.msg_iov = response_buffer_info;
			msg_header.msg_iovlen = 2;

			ssize_t read = recvmsg(conn_fd, &msg_header, 0);
			gettimeofday(&time_tmp, NULL);
			suseconds_t response_instant = time_tmp.tv_usec;

			response_time = response_instant - sent_instant;

			if (read <= 0)
			{
				fprintf(stderr, "ft_ping: %s: %s\n", cmd.address.data, strerror(errno));
				exit(2);
			}

			size_t icmphdr_offset = (((ipv4_header[0]) & 0x0F) - 5) * 4;
			size_t icmp_data_offset = icmphdr_offset + sizeof(struct icmphdr);

			response_icmphdr = (struct icmphdr *)(response_data + (icmphdr_offset));
			response_payload = response_data + icmp_data_offset;

			if (response_icmphdr->un.echo.id == getpid() && response_icmphdr->type == 0)
				break;
		}


		printf("Received ICMP response in %lu.%.3lums: { type: %d, code: %d, id: 0x%X, sequence: %u, payload: %s }\n",
			response_time / 1000,
			response_time % 1000,
			response_icmphdr->type,
			response_icmphdr->code,
			response_icmphdr->un.echo.id,
			response_icmphdr->un.echo.sequence,
			(char *)response_payload
		);

		sleep(1);
	}

	return 0;
}
