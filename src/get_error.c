/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clsaad <clsaad@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 16:51:42 by clsaad            #+#    #+#             */
/*   Updated: 2023/06/06 17:05:53 by clsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_error.h"

inline static const char	*type3(uint16_t code)
{
	const char	*txt[] = {
		"Destination Net Unreachable",
		"Destination Host Unreachable",
		"Destination Protocol Unreachable",
		"Destination Port Unreachable",
		"Frag needed and DF set",
		"Source Route Failed",
		"Destination Net Unknown",
		"Destination Host Unknown",
		"Source Host Isolated",
		"Destination Net Prohibited",
		"Destination Host Prohibited",
		"Destination Net Unreachable for Type of Service",
		"Destination Host Unreachable for Type of Service",
		"Packet filtered",
		"Precedence Violation",
		"Precedence Cutoff",
	};

	if (code <= 15)
		return (txt[code]);
	return ("Unknown error");
}

inline static const char	*type5(uint16_t code)
{
	if (code == 0)
		return ("Redirect Network");
	else if (code == 1)
		return ("Redirect Type of Service and Network");
	else if (code == 2)
		return ("Redirect Host");
	else if (code == 3)
		return ("Redirect Type of Service and Host");
	else
		return ("Unknown redirection");
}

const char	*get_error(uint16_t type, uint16_t code)
{
	if (type == 3)
		return (type3(code));
	else if (type == 4)
		return ("Source Quench");
	else if (type == 5)
		return (type5(code));
	else if (type == 11)
	{
		if (code == 0)
			return ("Time to live exceeded");
		else if (code == 1)
			return ("Frag reassembly time exceeded");
		else
			return ("Unknown Time Exceeded Message code");
	}
	else if (type == 12 && code == 0)
		return ("Parameter problem: pointer");
	else if (type == 13 && code == 0)
		return ("Parameter problem: pointer");
	return ("Unknown error");
}
