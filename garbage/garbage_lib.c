/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_lib.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 05:32:25 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 15:15:26 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*grbg_strdup(t_shell *shell, char *s)
{
	size_t	len; // stringin uzunluğunu tutacak değişken
	size_t	i; // string içindeki karakterlerle tek tek gezerken kullanılacak index
	char	*dest; // alan ayrılmış halini tutacak değişken

	i = 0; // index 0 dan başlatılır
	len = ft_strlen(s) + 1; // stirngin uzunluğu + NULL değeri atılır len olarak
	dest = get_grbg(shell, len, sizeof(char)); // hesaplanan len tane char boyutunda alan ayrılır yani len * sizeof(char)
	if (dest == NULL) // eğer yer ayrılırken hata olduysa
		return (NULL); // NULL döndür (ekrana allocation hatası mesajını bu fonksiyonun kullanıldığı yerlerde kontrol edip yazacağız)
	while (s[i] != '\0') // stringin sonuna kadar ilerle
	{
		dest[i] = s[i]; // içeriğindeki karakteri kopyala
		i++; // sonraki karaktere geç
	}
	dest[i] = '\0'; // sonuna NULL koy
	return (dest); // stringin allocation edilmiş halini döndür 
}

char	*grbg_substr(t_shell *shell, char *s, unsigned int start,
		size_t len) // gönderilen stringin start indexinden sonraki len kadar karakterini döndüren fonksiyon
{
	size_t	i; // string içinde gezmek için index
	char	*dest; // stringin istenilen kısmını tutacak değişken

	i = 0; // index 0 dan başla
	if (ft_strlen(s) < start) // eğer stringin boyutu başlanması istenilen indexten küçükse
		return (grbg_strdup(shell, "")); // NULL döndür ve çık
	if (ft_strlen(s + start) < len) // eğer başlanılacak indexten sonrasında kalan karakter sayısı kopyalanması istenilen len den azsa
		len = ft_strlen(s + start); // kopyalanacak karakter sayısını kalan karakter sayısına düşürür
	dest = get_grbg(shell, len + 1, sizeof(char)); // istenilen uzunluk kadar karakter alanı açılır hafızada
	if (!dest) // eğer alan ayrılmada sorun olduysa
		return (NULL); // NULL döndür ve çık
	while (i < len) // kopyalanması istenen uzunluk kadar string içinde gez
	{
		dest[i] = s[start + i]; // deste stringin start karakterinden sonraki karakterleri aktar
		i++; // sonraki karaktere geç
	}
	dest[i] = '\0'; // kopyalanmış halinin sonuna NULL koy
	return (dest); // istenilen değişkeni döndür
}

char	*grbg_strjoin(t_shell *shell, char *s1, char *s2) // gönderilen stringin sonuna diğer gönderilen stringi ekleyen fonksiyon
{
	size_t	len; // iki stringin birleşmiş halinin uzunluğunu tutacak değişken
	char	*dest; // iki stringin birleşmiş halini  tutacak string

	len = ft_strlen(s1) + ft_strlen(s2) + 1; // s1 uzunluğu + s2 uzunluğu + 1 kapanış karakteri NULL için
	dest = get_grbg(shell, sizeof(char), len); // birleşmiş hallerinin uzunluğu kadar hafızada yer ayrılır
	if (!dest) // eğer yer ayırmada sorun olduysa
		return (NULL); // NULL döndür ve çık
	ft_strlcpy(dest, s1, ft_strlen(s1) + 1); // stringe s1 stringini aktar
	ft_strlcat(dest, s2, len); // içine s1 aktarılmış stringin sonuna da s2 yi aktar
	return (dest); // birleşmiş hallerini döndür
}
