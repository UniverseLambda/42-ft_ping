/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:19:11 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/27 13:52:53 by clsaad           ###   ########.fr       */
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
	fprintf(stderr, "Usage: ft_ping [OPTION...] HOST ...\n");
	fprintf(stderr, "Send ICMP ECHO_REQUEST packets to network hosts.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, " Options valid for all request types:\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  -v,                        verbose output\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  -?                         give this help list\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Report bugs to <bug-inetutils@gnu.org>.\n");
	exit(2);
}

static void	set_opt_flag(unsigned int *flags, char c)
{
	t_command_flag	selected_flag;

	if (c == '?')
		selected_flag = CF_HELP;
	else if (c == 'v')
		selected_flag = CF_VERBOSE;
	else if (c == '-' && (*flags & CF_MINUS) == 0)
		selected_flag = CF_MINUS;
	else
	{
		fprintf(stderr, "ft_ping: invalid option -- '%c'\n", c);
		display_help();
	}
	*flags |= selected_flag;
}

static t_command	parse_args(char **argv)
{
	t_command	result;
	t_string	s;

	result.flags = 0;
	result.address_total = 0;
	if (argv[1] == NULL)
		err_exit("usage error: Destination address required\n");
	while (*(++argv))
	{
		result.flags &= ~CF_MINUS;
		s = string_new(*argv);
		if (string_char_at(&s, 0) == '-')
			string_foreach(&s, (t_foreach_handler)set_opt_flag, &result.flags);
		else if (result.address_total >= 512)
			err_exit("usage error: Address count limit exceeded (512)\n");
		else
			result.address[result.address_total++] = s;
	}
	if (!result.address_total && !(result.flags & CF_HELP))
		err_exit("usage error: Destination address required\n");
	return (result);
}

t_command	ftp_command(char **argv)
{
	t_command	cmd;

	cmd = parse_args(argv);
	if (cmd.flags & CF_HELP)
	{
		display_help();
	}
	return (cmd);
}
