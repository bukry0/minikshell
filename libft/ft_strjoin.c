/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saskin <saskin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 16:29:30 by saskin            #+#    #+#             */
/*   Updated: 2024/10/21 16:18:53 by saskin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		s1len;
	int		s2len;
	char	*newsrc;
	int		i;
	int		j;

	i = 0;
	j = 0;
	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	newsrc = (char *)malloc((s1len + s2len + 1) * sizeof(char));
	if (newsrc == NULL)
		return (NULL);
	while (s1[i])
	{
		newsrc[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		newsrc[j + i] = s2[j];
		j++;
	}
	newsrc[i + j] = '\0';
	return (newsrc);
}
