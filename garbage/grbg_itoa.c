/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grbg_itoa.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 05:38:26 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 15:05:50 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	get_len(long n)
{
	size_t	len;

	len = 0;
	if (n < 0)
	{
		len += 1;
		n = -n;
	}
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static char	*fill_the_array(int len, char *dest, int n)
{
	int	i;

	i = 0;
	if (n < 0)
	{
		dest[0] = '-';
		n = n * (-1);
		i = 1;
	}
	dest[len] = '\0';
	len -= 1;
	while (len >= i)
	{
		dest[len] = (n % 10) + '0';
		n /= 10;
		len--;
	}
	return (dest);
}

char	*grbg_itoa(t_shell *shell, int n)
{
	char	*dest;
	size_t	len;
	long	num;

	if (n == -2147483648)
		return (dest = grbg_strdup(shell, "-2147483648"));
	if (n == 0)
		return (dest = grbg_strdup(shell, "0"));
	num = n;
	len = get_len(num);
	dest = get_grbg(shell, len + 1, sizeof(char));
	if (!dest)
		return (NULL);
	dest = fill_the_array(len, dest, num);
	return (dest);
}
