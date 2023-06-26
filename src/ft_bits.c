/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bits.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 18:11:28 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/26 17:30:43 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/ft_bits.h"

static inline size_t	array_idx(size_t len, uint64_t seq)
{
	return (seq / 64 / len);
}

static inline uint64_t	bit_mask(uint64_t seq)
{
	return ((uint64_t)0x01 << (seq % 64));
}

bool	is_bit_set(const uint64_t *bits_array, size_t len, uint64_t seq)
{
	return (!!(bits_array[array_idx(len, seq)] & bit_mask(seq)));
}

void	set_bit(uint64_t *bits_array, size_t len, uint64_t seq)
{
	bits_array[array_idx(len, seq)] |= bit_mask(seq);
}

void	unset_bit(uint64_t *bits_array, size_t len, uint64_t seq)
{
	bits_array[array_idx(len, seq)] &= ~bit_mask(seq);
}
