/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_util.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:17:56 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/06 17:04:01 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	*ft_memset(void *dest, int v, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((char *)dest)[i] = (char)v;
		++i;
	}
	return (dest);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((char *)dest)[i] = ((char *)src)[i];
		++i;
	}
	return (dest);
}

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				index;
	const unsigned char	*c1 = s1;
	const unsigned char	*c2 = s2;

	if (s1 == s2)
		return (0);
	index = 0;
	while (index < n)
	{
		if (c1[index] != c2[index])
		{
			return (c1[index] - c2[index]);
		}
		++index;
	}
	return (0);
}
