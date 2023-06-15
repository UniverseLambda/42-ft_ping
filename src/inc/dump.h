/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dump.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 15:10:27 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/15 14:46:01 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DUMP_H
# define DUMP_H

# include <unistd.h>
# include <stdint.h>

void	dump_packet(uint8_t *iphdr, uint8_t *inner_icmp);

#endif // DUMP_H