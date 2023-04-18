/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:17:34 by clsaad            #+#    #+#             */
/*   Updated: 2023/04/18 16:55:38 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/ft_string.h"
#include <stdlib.h>

t_string	string_new(char *data)
{
	size_t	len;

	len = 0;
	while (data[len])
		++len;
	return ((t_string){
		.data = data, .len = len
	});
}

char	string_char_at(t_string *self, size_t idx)
{
	if (idx >= self->len)
		exit(2);
	return (self->data[idx]);
}

size_t	string_len(t_string *self)
{
	return (self->len);
}

t_string	string_slice(t_string *self, size_t start, size_t end)
{
	return ((t_string){
		.data = self->data + start,
		.len = start - end
	});
}

void	string_foreach(t_string *self, t_foreach_handler f, void *instance)
{
	size_t	index;

	index = 0;
	while (index < self->len)
	{
		f(instance, self->data[index]);
	}
}
