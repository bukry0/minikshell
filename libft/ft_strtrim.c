/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saskin <saskin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 20:55:00 by saskin            #+#    #+#             */
/*   Updated: 2024/10/23 12:44:35 by saskin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		len;
	char	*newarray;

	i = 0;
	while (s1[i] && ft_strchr(set, s1[i]) != 0)
		i++;
	len = ft_strlen(s1);
	while (len > i && s1[len - 1] && ft_strchr(set, s1[len - 1]) != 0)
		len--;
	newarray = (char *)malloc(sizeof(char) * (len - i + 1));
	if (!newarray)
		return (NULL);
	ft_memcpy(newarray, &s1[i], len - i);
	newarray[len - i] = '\0';
	return (newarray);
}
