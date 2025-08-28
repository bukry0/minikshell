/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_lib.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 21:10:07 by ecarlier          #+#    #+#             */
/*   Updated: 2025/08/28 11:46:46 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* custom libft functions with get_grbg instead of malloc or ft_calloc*/

char	*grbg_strdup(t_shell *shell, const char *s)
{
	size_t	len;
	size_t	i;
	char	*dest;

	i = 0;
	len = ft_strlen(s) + 1;
	dest = get_grbg(shell, len, sizeof(char));
	if (dest == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*grbg_substr(t_shell *shell, char const *s, unsigned int start,
		size_t len)
{	//s = boşluklara göre düzenlenmiş tam string
	//start = kelimenin başlangıç indeksi
	//len = kelimenin uzunluğu
	size_t	i;
	char	*dest;

	i = 0;
	if (ft_strlen(s) < start)
		return (grbg_strdup(shell, ""));
	if (ft_strlen(s + start) < len)
		len = ft_strlen(s + start);
	//Eğer start konumundan itibaren len kadar karakter yoksa (yani diziyi aşarsak), bu durumda kalan karakter sayısı kadar alınıyor. Yani diziyi taşırmıyoruz
	dest = get_grbg(shell, len + 1, sizeof(char));
	if (!dest)
		return (NULL);
	while (i < len)
	{
		dest[i] = s[start + i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*grbg_strjoin(t_shell *shell, char const *s1, char const *s2)
{
	size_t	len;
	char	*dest;

	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	dest = get_grbg(shell, sizeof(char), len);
	if (!dest)
		return (NULL);
	ft_strlcpy(dest, s1, ft_strlen(s1) + 1);
	ft_strlcat(dest, s2, len);
	return (dest);
}
