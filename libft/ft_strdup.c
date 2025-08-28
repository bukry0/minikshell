/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saskin <saskin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 17:27:46 by saskin            #+#    #+#             */
/*   Updated: 2024/10/21 15:55:20 by saskin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*newsrc;
	int		l;

	i = 0;
	l = 0;
	while (s1[l])
		l++;
	newsrc = (char *)malloc((l + 1) * sizeof(char));
	if (!(newsrc))
		return (NULL);
	while (s1[i])
	{
		newsrc[i] = s1[i];
		i++;
	}
	newsrc[i] = '\0';
	return (newsrc);
}
