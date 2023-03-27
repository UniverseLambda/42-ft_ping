/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:19:16 by clsaad            #+#    #+#             */
/*   Updated: 2023/03/27 17:23:09 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLI_H
# define CLI_H

# include "ft_string.h"

typedef enum e_command_flag
{
	CF_HELP		= 0x0001,
	CF_VERBOSE	= 0x0002,
} t_command_flag;

typedef struct s_command {
	unsigned int flags;
	t_string address;
} t_command;

t_command ftp_command(int argc, char **argv);

#endif // CLI_H
