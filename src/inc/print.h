/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 14:35:28 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/07 15:31:03 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_H
# define PRINT_H

# include <stdint.h>
# include "ft_result.h"

void	print_time(uint64_t time);
void	print_end_stats(void);
void	print_addr(t_sockaddr_res *sockaddr, char *response_ip);

#endif
