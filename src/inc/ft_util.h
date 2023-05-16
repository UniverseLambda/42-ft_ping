/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_util.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:17:55 by clsaad            #+#    #+#             */
/*   Updated: 2023/05/16 14:41:30 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_UTIL_H
# define FT_UTIL_H

# include <unistd.h>

void	*ft_memset(void *dest, int v, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);

#endif // FT_UTIL_H
