/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_result.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:18:09 by clsaad            #+#    #+#             */
/*   Updated: 2023/04/19 10:13:44 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RESULT_H
# define FT_RESULT_H

# include <unistd.h>
# include <stdbool.h>

union u_resultable
{
	char	data[128];
	int		error_code;
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
