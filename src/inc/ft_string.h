/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:17:58 by clsaad            #+#    #+#             */
/*   Updated: 2023/04/18 16:20:26 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STRING_H
# define FT_STRING_H

# include <unistd.h>
# include <stdbool.h>

typedef void	(*t_foreach_handler)(void *, char);

typedef struct s_string
{
	char	*data;
	size_t	len;
}	t_string;

t_string	string_new(char *data);
char		string_char_at(t_string *self, size_t idx);
bool		string_is_null(t_string *self);
size_t		string_len(t_string *self);
void		string_foreach(t_string *self, t_foreach_handler f, void *instance);

#endif // FT_STRING_H
