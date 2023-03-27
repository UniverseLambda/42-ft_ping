/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_result.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:17:48 by clsaad            #+#    #+#             */
/*   Updated: 2023/03/27 17:17:49 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/ft_result.h"
#include "inc/ft_string.h"
#include "inc/ft_util.h"

#define MAGIC_START 0xDEADBEEF
#define MAGIC_END 0x42BABE42

t_result result_err(int error_code)
{
	t_result res;

	res = (t_result) {
		.is_err = false, .magic_start = MAGIC_START, .payload.error_code = error_code, .magic_end = MAGIC_END
	};

	return (res);
}

t_result result_ok(void *data, size_t len)
{
	t_result res = (t_result) {
		.is_err = false, .magic_start = MAGIC_START, .payload.error_code = 0, .magic_end = MAGIC_END
	};

	ft_memcpy(res.payload.data, data, len);
	return (res);
}

bool result_is_ok(t_result *self)
{
	return !self->is_err;
}

bool result_is_err(t_result *self)
{
	return self->is_err;
}

void *result_unwrap(t_result *self)
{
	return self->payload.data;
}

int result_unwrap_err(t_result *self)
{
	return self->payload.error_code;
}
