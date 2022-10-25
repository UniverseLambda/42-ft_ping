#define _POSIX_C_SOURCE 200112L

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

#include "inc/result.h"
#include "inc/string.h"

typedef enum CommandFlag {
	CF_HELP		= 0x0001,
	CF_VERBOSE	= 0x0002,
} CommandFlag;

typedef struct Command {
	unsigned int flags;
	String address;
} Command;

__attribute__ ((__noreturn__)) static void display_help() {
	fprintf(stderr, "\n");
	fprintf(stderr, "Usage\n");
	fprintf(stderr, "ft_ping [options] <destination>\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  <destination>      dns name or ip address\n");
	fprintf(stderr, "  -h                 print help and exit\n");
	fprintf(stderr, "  -v                 verbose output\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "For more details see ping(8).\n");
	exit(2);
}

static void *ft_memset(void *dest, int v, size_t len) {
	for (size_t i = 0; i < len; ++i)
		((char *)dest)[i] = (char)v;
	return dest;
}

static void *ft_memcpy(void *dest, const void *src, size_t len) {
	for (size_t i = 0; i < len; ++i)
		((char *)dest)[i] = ((char *)src)[i];
	return dest;
}

static void make_icmp_packet(struct icmphdr *icmp_header, String payload, char *dest) {
	ft_memcpy(dest, icmp_header, sizeof(*icmp_header));

	ft_memcpy(dest + sizeof(*icmp_header), payload.data, payload.len);
}

static CommandFlag get_opt_flag(char c) {
	printf("FLAG: %c\n", c);

	switch (c)
	{
	case 'h': return CF_HELP;
	case 'v': return CF_VERBOSE;
	}

	fprintf(stderr, "ft_ping: invalid option -- '%c'\n", c);
	display_help();
}

static Command parse_args(int argc, char **argv) {
	Command result = {0};
	bool has_address = false;

	if (argc == 1) {
		fprintf(stderr, "ft_ping: usage error: Destination address required\n");
		exit(1);
	}

	for (int i = 1; i < argc; ++i) {
		String arg = string_new(argv[i]);

		if (string_char_at(&arg, 0) == '-') {
			for (size_t char_idx = 1; char_idx < string_len(&arg); ++char_idx) {
				result.flags |= get_opt_flag(string_char_at(&arg, char_idx));
			}
		} else {
			if (has_address) {
				display_help(argv[0]);
			}

			has_address = true;
			result.address = arg;
		}
	}

	if (!has_address && !(result.flags & CF_HELP)) {
		fprintf(stderr, "ft_ping: usage error: Destination address required\n");
		exit(1);
	}

	return result;
}

static struct addrinfo *resolve_host(String host) {
	struct addrinfo hints = {0};
	struct addrinfo *result;

	hints.ai_family = AF_INET;			/* Allow IPv4 only */
	hints.ai_socktype = SOCK_RAW;		/* Datagram socket */
	hints.ai_protocol = IPPROTO_ICMP;	/* ICMP protocol */

	printf("Resolving \"%s\"\n", host.data);

	int gai_ret_val = getaddrinfo(host.data, NULL, &hints, &result);

	if (gai_ret_val != 0) {
		fprintf(stderr, "ft_ping: %s: %s\n", host.data, gai_strerror(gai_ret_val));
		exit(1);
	}

	return result;
}

uint16_t compute_checksum(void *data, size_t data_len) {
	size_t word_len = data_len / 2;
	uint32_t sum = 0xFFFF;

	for (size_t i = 0; i < word_len; ++i) {
		sum += ((uint16_t *)data)[i];

		if (sum > 0xFFFF) {
			sum -= 0XFFFF;
		}
	}

	return sum;
}

int main(int argc, char **argv) {
	Command cmd = parse_args(argc, argv);

	if (cmd.flags & CF_HELP) {
		display_help();
	}

	struct addrinfo *current;
	struct addrinfo *resolved = resolve_host(cmd.address);
	struct sockaddr selected_address = {0};
	size_t selected_addresslen = 0;
	int conn_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	int sequence = 0;

	for (current = resolved; current != NULL; current = current->ai_next) {
		if (conn_fd == -1) {
			// FIXME: check for errno usage for subject compliance
			fprintf(stderr, "ft_ping: %s: %s\n", cmd.address.data, strerror(errno));
			exit(2);
		}

		selected_address = *(current->ai_addr);
		selected_addresslen = current->ai_addrlen;

		break;
	}

	freeaddrinfo(resolved);

	if (conn_fd == -1) {
		fprintf(stderr, "ft_ping: %s: Host not found\n", cmd.address.data);
	}

	printf("PING %s 56(84) bytes\n", cmd.address.data);

	struct icmphdr header = {0};
	header.type = ICMP_ECHO;
	header.un.echo.id = getpid();

	char buffer[1024];

	String data = string_new("MESSAGE!");

	for (;;) {
		char response_origin[255];
		char response_buffer[255];
		ft_memset(response_origin, 0, sizeof(response_origin));
		ft_memset(response_buffer, 0, sizeof(response_buffer));

		struct msghdr msg_header = {0};
		struct icmphdr response_header = {0};

		struct iovec response_buffer_info[2] = {
			(struct iovec) { .iov_base =  response_buffer, .iov_len = sizeof(response_buffer)},
			(struct iovec) { .iov_base = &response_header, .iov_len = sizeof(response_header)}
		};

		header.un.echo.sequence = ++sequence;

		make_icmp_packet(&header, data, buffer);

		uint16_t cs = ~compute_checksum(buffer, sizeof(header) + data.len);


		// Properly setting checksum without re-copying the whole packet into the buffer
		((uint16_t *)buffer)[1] = cs;

		int written = sendto(conn_fd, buffer, sizeof(header) + data.len, 0, &selected_address, selected_addresslen);

		if (written <= 0) {
			fprintf(stderr, "ft_ping: %s: %s\n", cmd.address.data, strerror(errno));
			exit(2);
		}

		msg_header.msg_name = response_origin;
		msg_header.msg_namelen = sizeof(response_origin);
		msg_header.msg_iov = response_buffer_info;
		msg_header.msg_iovlen = 2;

		ssize_t read = recvmsg(conn_fd, &msg_header, 0);

		if (read <= 0) {
			fprintf(stderr, "ft_ping: %s: %s\n", cmd.address.data, strerror(errno));
			exit(2);
		}

		printf("Received ICMP response: { id: %d, sequence: %d }\n", response_header.un.echo.id, response_header.un.echo.sequence);
	}

	return 0;
}
