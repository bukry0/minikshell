/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saskin <saskin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 18:52:05 by saskin            #+#    #+#             */
/*   Updated: 2024/10/18 15:41:20 by saskin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_digitcount(int n, int sign)
{
	int	i;

	i = 0;
	if (n == 0)
		i = 1;
	while (n != 0)
	{
		i++;
		n = n / 10;
	}
	if (sign < 0)
		i++;
	return (i);
}

char	*ft_itoa(int n)
{
	char	*str;
	long	number;
	int		sign;
	int		digitcount;

	number = n;
	sign = 1;
	if (number < 0)
		sign = -1;
	number = sign * number;
	digitcount = ft_digitcount(number, sign);
	str = (char *)malloc(sizeof(char) * digitcount + 1);
	if (!str)
		return (0);
	str[digitcount] = '\0';
	while (--digitcount >= 0)
	{
		str[digitcount] = (number % 10) + '0';
		number = number / 10;
	}
	if (sign == -1)
		str[0] = '-';
	return (str);
}
