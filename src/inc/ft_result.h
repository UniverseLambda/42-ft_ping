/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_result.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:18:09 by clsaad            #+#    #+#             */
/*   Updated: 2023/03/27 17:23:28 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RESULT_H
# define FT_RESULT_H

#include <unistd.h>
#include <stdbool.h>

union u_result_payload
{
	char data[128];
	int error_code;
};

typedef struct s_result
{
	bool is_err;
	unsigned int magic_start;
	union u_result_payload payload;
	unsigned int magic_end;
} t_result;

t_result result_err(int error_code);
t_result result_ok(void *data, size_t len);

bool result_is_ok(t_result *self);
bool result_is_err(t_result *self);

void *result_unwrap(t_result *self);
int result_unwrap_err(t_result *self);

#endif // FT_RESULT_H
