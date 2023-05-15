/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_result.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:18:09 by clsaad            #+#    #+#             */
/*   Updated: 2023/05/15 15:28:10 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RESULT_H
# define FT_RESULT_H

# include <stdbool.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_sockaddr_res
{
	struct sockaddr	sock_addr;
	size_t			sock_addr_len;
}	t_sockaddr_res;

union u_resultable
{
	t_sockaddr_res	sock_addr;
	suseconds_t		suseconds;
	int				error_code;
};

typedef struct s_result
{
	bool					is_err;
	unsigned int			magic_start;
	union u_resultable		payload;
	unsigned int			magic_end;
}	t_result;

t_result	result_err(int error_code);
t_result	result_ok(union u_resultable data);

bool		result_is_ok(t_result *self);
bool		result_is_err(t_result *self);

#endif // FT_RESULT_H
