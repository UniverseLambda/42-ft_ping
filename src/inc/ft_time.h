/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_time.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 10:32:27 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/06 15:10:03 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TIME_H
# define FT_TIME_H

# include <stdint.h>

uint64_t	now_micro(void);
uint64_t	now_sec(void);

#endif // FT_TIME_H