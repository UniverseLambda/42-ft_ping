/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:17:50 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/06 16:39:20 by clsaad           ###   ########.fr       */
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
#include "inc/init.h"
#include "inc/ft_result.h"
#include "inc/ft_sqrt.h"
#include "inc/ft_string.h"
#include "inc/ft_time.h"
#include "inc/ft_util.h"
#include "inc/ft_signal.h"
#include "inc/ping_stats.h"

typedef struct icmphdr t_icmphdr;
typedef struct s_iter_info
{
	t_sockaddr_res	resp_origin;
	unsigned char	ipv4_header[20];
	unsigned char	response_data[512];
	struct iovec	response_buffer_info[2];
	t_icmphdr		*resp_icmphdr;
	struct msghdr	msg_header;
	uint64_t		sent_instant;
	bool			responded;
	uint64_t		responded_time;
	const char		*error_message;
}	t_iter_info;

int	usleep(useconds_t usec);

static struct sockaddr_in	*sockaddr_in(void *ptr)
{
	return (ptr);
}

static void	make_icmp_packet(
	struct icmphdr *icmp_header, t_string payload, char *dest)
{
	const uint64_t	timestamp = now_sec();

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

static const char	*get_error(uint16_t type, uint16_t code)
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
	const uint64_t	left = time / 1000;
	const uint64_t	right = time % 1000;
	char			buffer[7];

	if (left >= 100)
		printf("%lums\n", left);
	else
	{
		ft_memset(buffer, 0, 6);
		snprintf(buffer, 7, "%lu.%.3lu", left, right);
		printf("%.*sms\n", 5 - (left != 0), buffer);
	}
}

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
			&sa.sock_addr, sa.sock_addr_len))
		return (result_err(errno));
	pstats_sent();
	return (result_ok((union u_resultable)0));
}

static char	*time_to_printable(char *buf, uint64_t time)
{
	if (time == 0)
		return ("0");
	snprintf(buf, 32, "%lu.%lu", time / 1000, time % 1000);
	return (buf);
}

static void	print_end_stats(void)
{
	const struct s_ping_stats	stats = pstats_get();
	char						tmp_buf[32];
	uint64_t					mdev;
	uint64_t					average;
	uint64_t					sq_average;

	printf("\n--- %s ping statistics ---\n", stats.user_addr.data);
	printf("%zu packets transmitted, %zu received, "
		"%zu%% packet loss, time %ldms\n",
		stats.sent,
		stats.received,
		100 - ((stats.received * 100) / stats.sent),
		(now_micro() - stats.start) / 1000);
	if (stats.received == 0)
		return ;
	average = stats.rtt_sum / stats.received;
	sq_average = stats.rtt_sq_sum / stats.received;
	mdev = ft_sqrt(sq_average - (average * average));
	printf("rtt min/avg/max/mdev = %s/", time_to_printable(tmp_buf, stats.min));
	printf("%s/", time_to_printable(tmp_buf, average));
	printf("%s/", time_to_printable(tmp_buf, stats.max));
	printf("%s ms\n", time_to_printable(tmp_buf, mdev));
}

static char	*resolve_cache_addr(const t_sockaddr_res *sockaddr)
{
	static t_sockaddr_res	last_sockaddr;
	static char				buf[257];

	if (ft_memcmp(&sockaddr->sock_addr, &last_sockaddr, sockaddr->sock_addr_len
			+ sizeof(sockaddr->sock_addr_len)) == 0)
		return (buf);
	last_sockaddr = *sockaddr;
	ft_memset(buf, 0, sizeof(buf));
	if (getnameinfo(&last_sockaddr.sock_addr, sizeof(last_sockaddr.sock_addr),
			buf, sizeof(buf), NULL, 0, NI_NAMEREQD))
		buf[0] = '\0';
	return (buf);
}

static void	print_addr(t_sockaddr_res *sockaddr, char *response_ip)
{
	const char	*addr = resolve_cache_addr(sockaddr);

	if (!addr || !(addr[0]))
		printf("%s", response_ip);
	else
		printf("%s (%s)", addr, response_ip);
}

static void	received_stats(t_iter_info *iter, uint16_t sequence)
{
	char		response_ip[INET_ADDRSTRLEN];
	uint16_t	packet_len;

	if (!iter->error_message)
		pstats_responded(iter->responded_time);
	inet_ntop(AF_INET, &(sockaddr_in(&iter->resp_origin.sock_addr))->sin_addr,
		response_ip, INET_ADDRSTRLEN);
	if (iter->error_message)
	{
		printf("From ");
		print_addr(&iter->resp_origin, response_ip);
		printf(" icmp_seq=%u %s\n", sequence, iter->error_message);
	}
	else
	{
		packet_len = (((uint16_t)iter->ipv4_header[2] << 8)
				| iter->ipv4_header[3]) - ((iter->ipv4_header[0] & 0x0F) * 4);
		printf("%u bytes from ", packet_len);
		print_addr(&iter->resp_origin, response_ip);
		printf(": icmp_seq=%u ttl=%u time=",
			iter->resp_icmphdr->un.echo.sequence, iter->ipv4_header[8]);
		print_time(iter->responded_time);
	}
}

void	new_iteration(t_iter_info *iter)
{
	signal(SIGALRM, signal_handler);
	iter->sent_instant = now_micro();
	ft_memset(&(iter->resp_origin), 0, sizeof(iter->resp_origin));
	ft_memset(&(iter->ipv4_header), 0, 20);
	ft_memset(&(iter->response_data), 0, 512);
	iter->response_buffer_info[0] = (struct iovec)
	{.iov_base = &(iter->ipv4_header), .iov_len = 20};
	iter->response_buffer_info[1] = (struct iovec)
	{.iov_base = &(iter->response_data), .iov_len = 512};
	iter->error_message = NULL;
	iter->responded = true;
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
	return (true);
}

static void	new_listen_try(t_iter_info *iter)
{
	iter->msg_header.msg_name = &(iter->resp_origin.sock_addr);
	iter->msg_header.msg_namelen = sizeof(iter->resp_origin.sock_addr);
	iter->msg_header.msg_iov = (iter->response_buffer_info);
	iter->msg_header.msg_iovlen = 2;
}

static size_t	get_icmphdr_offset(char *ipv4_header)
{
	return ((ipv4_header[0] & 0x0F) * 4);
}

static bool	is_ours(char *icmp_buf, uint16_t sequence)
{
	const t_icmphdr	*icmp_hdr = (t_icmphdr *)icmp_buf;
	const char		*sub_hdr = icmp_buf + 8;

	if (icmp_hdr->type == 0 || icmp_hdr->type == 8)
		return (icmp_hdr->un.echo.id == getpid()
			&& icmp_hdr->un.echo.sequence == sequence);
	else if (icmp_hdr->type < 13)
		return (is_ours(sub_hdr + (get_icmphdr_offset(sub_hdr)), sequence));
	return (false);
}

static bool	handle_packet(t_iter_info *iter, uint16_t sequence)
{
	if (!is_ours((char *)(iter->resp_icmphdr), sequence))
		return (false);
	if (iter->resp_icmphdr->type == 0)
	{
		if (iter->resp_icmphdr->un.echo.id == getpid()
			&& iter->resp_icmphdr->un.echo.sequence == sequence)
			return (true);
	}
	else if (iter->resp_icmphdr->type != 8)
	{
		iter->error_message
			= get_error(iter->resp_icmphdr->type, iter->resp_icmphdr->code);
		return (true);
	}
	return (false);
}

static bool	try_listen_for_answer(
	const t_initedping *ping, t_iter_info *iter, uint16_t sequence)
{
	size_t	icmp_off;

	if (recvmsg(ping->conn_fd, &iter->msg_header, MSG_WAITALL) <= 0)
	{
		if (errno == EINTR)
		{
			iter->responded = false;
			return (true);
		}
		fprintf(stderr,
			"ft_ping: %s: %s\n", ping->address.data, strerror(errno));
		exit(2);
	}
	iter->resp_origin.sock_addr_len = iter->msg_header.msg_namelen;
	iter->responded_time = now_micro() - iter->sent_instant;
	icmp_off = get_icmphdr_offset((char *)iter->ipv4_header) - (4 * 5);
	iter->resp_icmphdr = (t_icmphdr *)(iter->response_data + (icmp_off));
	return (handle_packet(iter, sequence));
}

static void	start_ping(const t_initedping *ping)
{
	uint16_t	sequence;
	t_iter_info	iter;

	sequence = 0;
	printf("PING %s 56(84) bytes\n", ping->address.data);
	while (1)
	{
		if (!try_send_message(ping, &sequence))
			break ;
		new_iteration(&iter);
		alarm(1);
		while (1)
		{
			new_listen_try(&iter);
			if (try_listen_for_answer(ping, &iter, sequence))
				break ;
		}
		alarm(0);
		if (*last_signal() == SIGINT)
			break ;
		else if (iter.responded)
			received_stats(&iter, sequence);
		if (*last_signal() != SIGINT && iter.responded_time < 1000000)
			usleep(1000000 - iter.responded_time);
	}
}

int	main(int argc, char **argv)
{
	const t_initedping	ping = ping_init(argv);

	(void)argc;
	start_ping(&ping);
	print_end_stats();
	return (0);
}
