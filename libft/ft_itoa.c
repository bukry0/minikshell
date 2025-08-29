/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 19:17:59 by bcili             #+#    #+#             */
/*   Updated: 2024/11/06 15:36:42 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_length(int n)
{
	int	i;

	i = 0;
	if (n <= 0)
		i++;
	while (n != 0)
	{
		i++;
		n = n / 10;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*str;
	long	nmb;
	int		len;

	nmb = n;
	len = ft_length(nmb);
	if (n < 0)
		nmb = -nmb;
	str = (char *)malloc(sizeof(char) * len + 1);
	if (!str)
		return (0);
	str[len] = '\0';
	while (--len >= 0)
	{
		str[len] = (nmb % 10) + '0';
		nmb = nmb / 10;
	}
	if (n < 0)
		str[0] = '-';
	return (str);
}
