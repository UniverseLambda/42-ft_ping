/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:19:11 by clsaad            #+#    #+#             */
/*   Updated: 2023/04/18 16:41:59 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "inc/cli.h"
#include "inc/ft_string.h"

__attribute__ ((__noreturn__)) static void	err_exit(char *err)
{
	fprintf(stderr, "ft_ping: %s", err);
	exit(1);
}

__attribute__ ((__noreturn__)) static void	display_help(void)
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

static void	set_opt_flag(unsigned int *flags, char c)
{
	t_command_flag	selected_flag;

	if (c == 'h')
		selected_flag = CF_HELP;
	else if (c == 'v')
		selected_flag = CF_VERBOSE;
	else
	{
		fprintf(stderr, "ft_ping: invalid option -- '%c'\n", c);
		display_help();
	}
	*flags |= selected_flag;
}

// TODO: add support for arguments without '-'

static t_command	parse_args(int argc, char **argv)
{
	int			index;
	t_command	result;
	bool		has_address;
	t_string	s;

	index = 0;
	result.flags = 0;
	has_address = false;
	if (argc == 1)
		err_exit("ft_ping: usage error: Destination address required\n");
	while (++index < argc)
	{
		s = string_new(argv[index]);
		if (string_char_at(&s, 0) == '-')
			string_foreach(&s, (t_foreach_handler)set_opt_flag, &result.flags);
		else
		{
			if (has_address)
				display_help();
			has_address = true;
			result.address = s;
		}
	}
	if (!has_address && !(result.flags & CF_HELP))
		err_exit("ft_ping: usage error: Destination address required\n");
	return (result);
}

t_command	ftp_command(int argc, char **argv)
{
	t_command	cmd;

	cmd = parse_args(argc, argv);
	if (cmd.flags & CF_HELP)
	{
		display_help();
	}
	return (cmd);
}
