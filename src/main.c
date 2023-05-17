/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:17:50 by clsaad            #+#    #+#             */
/*   Updated: 2023/05/17 13:41:41 by clsaad           ###   ########.fr       */
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
#include <stdbool.h>

#include "inc/cli.h"
#include "inc/ft_result.h"
#include "inc/ft_sqrt.h"
#include "inc/ft_string.h"
#include "inc/ft_time.h"
#include "inc/ft_util.h"
#include "inc/ping_stats.h"

typedef struct s_initedping
{
	t_sockaddr_res	sockaddr;
	t_string		address;
	int				conn_fd;
}	t_initedping;

typedef struct s_iter_info
{
	struct sockaddr	response_origin;
	unsigned char	ipv4_header[20];
	unsigned char	response_data[512];
	struct iovec	response_buffer_info[2];
	struct icmphdr	*response_icmphdr;
	struct msghdr	msg_header;
	uint64_t		sent_instant;
	bool			responded;
	uint64_t		responded_time;
	const char		*error_message;
}	t_iter_info;

int usleep(useconds_t usec);

static void make_icmp_packet(struct icmphdr *icmp_header, t_string payload, char *dest)
{
	const uint64_t	timestamp = now_sec();

	ft_memcpy(dest, icmp_header, sizeof(*icmp_header));
	ft_memcpy(dest + sizeof(*icmp_header), payload.data, payload.len);
	ft_memcpy(dest + sizeof(*icmp_header), &timestamp, sizeof(timestamp));
}

static struct addrinfo *resolve_host(t_string host)
{
	struct addrinfo hints = {0};
	struct addrinfo *result;

	hints.ai_family = AF_INET;			/* Allow IPv4 only */
	hints.ai_socktype = SOCK_RAW;		/* Raw socket */
	hints.ai_protocol = IPPROTO_ICMP;	/* ICMP protocol */

	// TODO: Transparent IPv4/IPv6 support

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


static const char *get_error(uint16_t type, uint16_t code)
{
	if (type == 3)
	{
		if (code == 0)
			return ("Destination Net Unreachable");
		else if (code == 1)
			return ("Destination Host Unreachable");
		else if (code == 2)
			return ("Destination Protocol Unreachable");
		else if (code == 3)
			return ("Destination Port Unreachable");
		else if (code == 4)
			return ("Frag needed and DF set");
		else if (code == 5)
			return ("Source Route Failed");
		else if (code == 6)
			return ("Destination Net Unknown");
		else if (code == 7)
			return ("Destination Host Unknown");
		else if (code == 8)
			return ("Source Host Isolated");
		else if (code == 9)
			return ("Destination Net Prohibited");
		else if (code == 10)
			return ("Destination Host Prohibited");
		else if (code == 11)
			return ("Destination Net Unreachable for Type of Service");
		else if (code == 12)
			return ("Destination Host Unreachable for Type of Service");
		else if (code == 13)
			return ("Packet filtered");
		else if (code == 14)
			return ("Precedence Violation");
		else if (code == 15)
			return ("Precedence Cutoff");
	}
	else if (type == 4)
		return ("Source Quench");
	else if (type == 5)
	{
		if (code == 0)
			return ("Redirect Network");
		else if (code == 1)
			return ("Redirect Type of Service and Network");
		else if (code == 2)
			return ("Redirect Host");
		else if (code == 3)
			return ("Redirect Type of Service and Host");
		else
			return ("Unknown redirection");
	}
	else if (type == 5)
	{
		if (code == 0)
			return ("Redirect Network");
		else if (code == 1)
			return ("Redirect Type of Service and Network");
		else if (code == 2)
			return ("Redirect Host");
		else if (code == 3)
			return ("Redirect Type of Service and Host");
	}
	else if (type == 11)
	{
		if (code == 0)
			return ("Time to live exceeded");
		else if (code == 1)
			return ("Frag reassembly time exceeded");
		else
			return ("Unknown Time Exceeded Message code");
	}
	else if (type == 12 && code == 0)
		return ("Parameter problem: pointer");
	else if (type == 13 && code == 0)
		return ("Parameter problem: pointer");
	return ("Unknown error");
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

static t_result send_icmp_echo(int conn_fd, uint16_t *sequence, t_sockaddr_res sa)
{
	struct icmphdr	header = {0};
	char			buffer[512];
	const t_string	data = string_new("        Lorem ipsum dolor sit amet, consectetur posuere.");

	header.type = ICMP_ECHO;
	header.un.echo.id = getpid();
	header.un.echo.sequence = ++(*sequence);
	make_icmp_packet(&header, data, buffer);
	// Properly setting checksum without re-copying the whole packet into the buffer
	((uint16_t *)buffer)[1] = ~compute_checksum(buffer, sizeof(header) + data.len);
	if (0 > sendto(conn_fd, buffer, sizeof(header) + data.len, 0, &sa.sock_addr, sa.sock_addr_len))
		return (result_err(errno));
	pstats_sent();
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
	uint64_t					average;
	uint64_t					sq_average;

	printf("\n--- %s ping statistics ---\n", stats.user_addr.data);
	printf("%zu packets transmitted, %zu received, %zu%% packet loss, time %ldms\n",
		stats.sent,
		stats.received,
		100 - ((stats.received * 100) / stats.sent),
		(now_micro() - stats.start) / 1000);

	if (stats.received == 0)
		return;
	average = stats.rtt_sum / stats.received;
	sq_average = stats.rtt_sq_sum / stats.received;
	mdev = ft_sqrt(sq_average - (average * average));
	printf("rtt min/avg/max/mdev = %s/", time_to_printable(tmp_buf, stats.min));
	printf("%s/", time_to_printable(tmp_buf, average));
	printf("%s/", time_to_printable(tmp_buf, stats.max));
	printf("%s ms\n", time_to_printable(tmp_buf, mdev));
}

static char *resolve_cache_addr(const t_sockaddr_res *sockaddr)
{
	static t_sockaddr_res	last_sockaddr;
	static char buf[257];

	if (ft_memcmp(&sockaddr->sock_addr, &last_sockaddr, sockaddr->sock_addr_len + sizeof(sockaddr->sock_addr_len)) == 0)
		return (buf);
	last_sockaddr = *sockaddr;
	ft_memset(buf, 0, sizeof(buf));
	getnameinfo(&last_sockaddr.sock_addr, last_sockaddr.sock_addr_len, buf, sizeof(buf), NULL, 0, 0);
	return (buf);
}

static void received_stats(const t_initedping *ping, t_iter_info *iter, uint16_t sequence)
{
	char response_ip[INET_ADDRSTRLEN];
	uint16_t packet_len;

	if (!iter->error_message)
		pstats_responded(iter->responded_time);

	inet_ntop(AF_INET, &((struct sockaddr_in *)&ping->sockaddr.sock_addr)->sin_addr, response_ip, INET_ADDRSTRLEN);

	if (iter->error_message)
		printf("From %s (%s) icmp_seq=%u %s\n",
			resolve_cache_addr(&ping->sockaddr),
			response_ip,
			sequence,
			iter->error_message
		);
	else
	{
		packet_len = (((uint16_t)iter->ipv4_header[2] << 8) | iter->ipv4_header[3]) - ((iter->ipv4_header[0] & 0x0F) * 4);
		printf("%u bytes from %s (%s): icmp_seq=%u ttl=%u time=",
			packet_len,
			resolve_cache_addr(&ping->sockaddr),
			response_ip,
			iter->response_icmphdr->un.echo.sequence,
			iter->ipv4_header[8]
		);
		print_time(iter->responded_time);
	}
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

void new_iteration(t_iter_info *iter)
{
	signal(SIGALRM, signal_handler);
	iter->sent_instant = now_micro();
	ft_memset(&(iter->response_origin), 0, sizeof(struct sockaddr));
	ft_memset(&(iter->ipv4_header), 0, 20);
	ft_memset(&(iter->response_data), 0, 512);
	iter->response_buffer_info[0] = (struct iovec)
			{ .iov_base = &(iter->ipv4_header), .iov_len = 20};
	iter->response_buffer_info[1] = (struct iovec)
			{ .iov_base = &(iter->response_data), .iov_len = 512};
	iter->error_message = NULL;
	iter->responded = true;
}

static bool try_send_message(const t_initedping *ping, uint16_t *sequence)
{
	t_result send_res;

	send_res = send_icmp_echo(ping->conn_fd, sequence, ping->sockaddr);
	if (result_is_err(&send_res))
	{
		if (send_res.payload.error_code == EINTR)
			return (false);
		fprintf(stderr, "ft_ping: %s: %s\n", ping->address.data, strerror(send_res.payload.error_code));
		exit(2);
	}
	return (true);
}

static void new_listen_try(t_iter_info *iter)
{
	iter->msg_header.msg_name = &(iter->response_origin);
	iter->msg_header.msg_namelen = sizeof(iter->response_origin);
	iter->msg_header.msg_iov = (iter->response_buffer_info);
	iter->msg_header.msg_iovlen = 2;
}

static bool try_listen_for_answer(const t_initedping *ping, t_iter_info *iter, uint16_t sequence)
{
	ssize_t read;

	read = recvmsg(ping->conn_fd, &iter->msg_header, MSG_WAITALL);
	iter->responded_time = now_micro() - iter->sent_instant;
	if (read <= 0)
	{
		if (errno == EINTR)
		{
			iter->responded = false;
			return (true);
		}
		fprintf(stderr, "ft_ping: %s: %s\n", ping->address.data, strerror(errno));
		exit(2);
	}

	size_t icmphdr_offset = (((iter->ipv4_header[0]) & 0x0F) - 5) * 4;
	// size_t icmp_data_offset = icmphdr_offset + sizeof(struct icmphdr);

	iter->response_icmphdr = (struct icmphdr *)(iter->response_data + (icmphdr_offset));
	// response_payload = response_data + icmp_data_offset;

	// TODO: check if underlying packet is ours

	if (iter->response_icmphdr->type == 0)
	{
		if (iter->response_icmphdr->un.echo.id == getpid() && iter->response_icmphdr->un.echo.sequence == sequence)
			return (true);
	}
	else if (iter->response_icmphdr->type != 8)
	{
		iter->error_message = get_error(iter->response_icmphdr->type, iter->response_icmphdr->code);
		return (true);
	}
	return (false);
}

static void start_ping(const t_initedping *ping)
{
	uint16_t	sequence;
	t_iter_info	iter;

	sequence = 0;
	printf("PING %s 56(84) bytes\n", ping->address.data);
	while (1)
	{
		if (!try_send_message(ping, &sequence))
			break;
		new_iteration(&iter);
		alarm(1);
		while (1)
		{
			new_listen_try(&iter);
			if (try_listen_for_answer(ping, &iter, sequence))
				break;
		}
		alarm(0);
		if (*last_signal() == SIGINT)
			break;
		else if (iter.responded)
			received_stats(ping, &iter, sequence);
		if (*last_signal() != SIGINT && iter.responded_time < 1000000)
			usleep(1000000 - iter.responded_time);
	}
}

int main(int argc, char **argv)
{
	const t_initedping ping = ping_init(argc, argv);

	start_ping(&ping);
	print_end_stats();
	return (0);
}
