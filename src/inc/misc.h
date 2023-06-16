/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 17:16:49 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/16 13:15:10 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MISC_H
# define MISC_H

# include "ft_result.h"
# include "iter.h"

char	*resolve_cache_addr(const t_sockaddr_res *sockaddr);
void	received_stats(t_iter_info *iter, uint16_t sequence);
bool	handle_packet(t_iter_info *iter);

#endif