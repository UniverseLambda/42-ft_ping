/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bits.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 18:05:30 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/16 21:56:57 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_BITS_H
# define FT_BITS_H

# include <stdint.h>
# include <unistd.h>
# include <stdbool.h>

bool	is_bit_set(const uint64_t *bits_array, size_t len, uint64_t seq);
void	set_bit(uint64_t *bits_array, size_t len, uint64_t seq);
void	unset_bit(uint64_t *bits_array, size_t len, uint64_t seq);

#endif // FT_BITS_H
