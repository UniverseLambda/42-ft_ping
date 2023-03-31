/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:19:11 by clsaad            #+#    #+#             */
/*   Updated: 2023/03/27 17:59:23 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "inc/cli.h"
#include "inc/ft_string.h"

__attribute__ ((__noreturn__)) static void display_help()
{
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

static t_command_flag get_opt_flag(char c)
{
	if (c == 'h')
		return (CF_HELP);
	else if (c == 'v')
		return (CF_VERBOSE);
	fprintf(stderr, "ft_ping: invalid option -- '%c'\n", c);
	display_help();
}

static t_command parse_args(int argc, char **argv)
{
	int index;
	t_command result = {0};
	bool has_address = false;
	t_string arg;

	if (argc == 1)
	{
		fprintf(stderr, "ft_ping: usage error: Destination address required\n");
		exit(1);
	}
	index = 0;
	while (++index < argc)
	{
		arg = string_new(argv[index]);

		if (string_char_at(&arg, 0) == '-')
			for (size_t char_idx = 1; char_idx < string_len(&arg); ++char_idx)
				result.flags |= get_opt_flag(string_char_at(&arg, char_idx));
		else {
			if (has_address)
				display_help(argv[0]);
			has_address = true;
			result.address = arg;
		}
	}

	if (!has_address && !(result.flags & CF_HELP))
	{
		fprintf(stderr, "ft_ping: usage error: Destination address required\n");
		exit(1);
	}

	return result;
}

t_command ftp_command(int argc, char **argv)
{
	t_command cmd = parse_args(argc, argv);

	if (cmd.flags & CF_HELP)
		display_help();

	return (cmd);
}
