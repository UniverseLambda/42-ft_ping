/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:17:34 by clsaad            #+#    #+#             */
/*   Updated: 2023/03/27 17:17:41 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/ft_string.h"
#include <stdlib.h>

t_string string_new(char *data)
{
	size_t len;

	len = 0;
	while (data[len])
		++len;

	return (t_string) { .data = data, .len = len };
}

char string_char_at(t_string *self, size_t idx)
{
	if (idx >= self->len)
		exit(2);

	return self->data[idx];
}

size_t string_len(t_string *self)
{
	return self->len;
}

bool string_is_null(t_string *self)
{
	return !(self->data);
}
