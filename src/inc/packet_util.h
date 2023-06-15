/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet_util.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 13:38:47 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/14 14:50:53 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PACKET_UTIL_H
# define PACKET_UTIL_H

# include <stdint.h>
# include <stdbool.h>
# include <unistd.h>

# include "ft_string.h"

typedef struct icmphdr	t_icmphdr;

size_t				get_icmphdr_offset(char *ipv4_header);
bool				is_ours(char *icmp_buf, uint16_t sequence, bool recursive);
void				make_icmp_packet(
						t_icmphdr *icmp_header, t_string payload, char *dest);
uint16_t			compute_checksum(void *data, size_t data_len);

struct sockaddr_in	*sockaddr_in(void *ptr);

#endif // PACKET_UTIL_H