/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:17:58 by clsaad            #+#    #+#             */
/*   Updated: 2023/03/27 17:17:58 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STRING_H
# define FT_STRING_H

#include <unistd.h>
#include <stdbool.h>

typedef struct s_string
{
	char *data;
	size_t len;
} t_string;

t_string	string_new(char *data);
char	string_char_at(t_string *self, size_t idx);
bool	string_is_null(t_string *self);
size_t	string_len(t_string *self);

#endif // FT_STRING_H
