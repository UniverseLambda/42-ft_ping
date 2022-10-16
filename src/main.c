#define _POSIX_C_SOURCE 200112L

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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

// static Result resolve_host(String host) {
// 	struct addrinfo hints;

// 	hints.ai_family = AF_INET;			/* Allow IPv4 only */
// 	hints.ai_socktype = SOCK_DGRAM;		/* Datagram socket */
// 	hints.ai_flags = AI_PASSIVE;		/* For wildcard IP address */
// 	hints.ai_protocol = IPPROTO_ICMP;	/* Any protocol */
// 	hints.ai_canonname = NULL;
// 	hints.ai_addr = NULL;
// 	hints.ai_next = NULL;

// 	return RES_ERR(0);
// }

int main(int argc, char **argv) {
	// struct addrinfo *addr_list, *addr_info;

	Command cmd = parse_args(argc, argv);

	if (cmd.flags & CF_HELP) {
		display_help();
	}

	// int socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_ICMP);

	return 0;
}
