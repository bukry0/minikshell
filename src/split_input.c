/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 16:39:22 by ecarlier          #+#    #+#             */
/*   Updated: 2025/08/28 11:49:15 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
Counts the number of words in the given string `str`,
considering a set of separators defined by `sep`.
It handles cases where words might be enclosed within single or double quotes,
ensuring they are not counted separately.
If there's a mismatch in quotes, it returns -1 to indicate an error.
*/
static int	ft_count_words_ms(const char *str, char *sep, int count, int i)
{

	int		in_quote = 0;
	char	quote_char = 0;
	while (str[i] != '\0')
	{
		if (!ft_strchr(sep, str[i]))
		{
			count++;
			while ((!ft_strchr(sep, str[i]) || in_quote) && str[i] != '\0')//eğer ayraç karakterde değilsek veya tırnak içindeysek saymaya devam et
			{
				if (!quote_char && (str[i] == '\"' || str[i] == '\''))
					quote_char = str[i];
				if (str[i] == quote_char)
					in_quote = !in_quote;
				if (in_quote == 0)
					quote_char = 0;
				i++;
			}
			if (in_quote)
				return (-1);
		}
		else
			i++;
	}
	return (count);
}

/*
	i[0]
	i[1] : start_index
	i[2] : word_index
*/
char **ft_create_substrs(t_shell *shell, char **substrings, const char *input, char *separators)
{
	//substrings = kelime sayısı kadar yer açılan arr dizisi
	//input = boşluklara göre düzenlenmiş tam string
	//seperators = " "
	int current_index = 0;       // i[0]: input stringinde gezinmek için
	int start_index = 0;         // i[1]: kelimenin başladığı yer
	int substr_index = 0;        // i[2]: substrings dizisinde kaçıncı kelime

	int in_single_quote = 0;     // q[0]: tek tırnak içinde mi?
	int in_double_quote = 0;     // q[1]: çift tırnak içinde mi?

	while (input[current_index])
	{
		// Başlangıçta ayraç karakterleri (boşluk, |, >, < vs.) atla
		while (ft_strchr(separators, input[current_index]) && input[current_index] != '\0')
			current_index++;

		start_index = current_index; // Kelimenin başladığı yer

	/* 	Şu anda tek tırnak içinde isek, veya
		Şu anda çift tırnak içinde isek
		Karakter ayraç değilse (örneğin boşluk, |, >, < gibi değilse) */
		while (( !ft_strchr(separators, input[current_index]) || in_single_quote || in_double_quote) && input[current_index])
		{
			// Tek tırnak açılışı/kapanışı
			if (!in_double_quote && input[current_index] == '\'')
				in_single_quote = !in_single_quote;

			// Çift tırnak açılışı/kapanışı
			if (!in_single_quote && input[current_index] == '\"')
				in_double_quote = !in_double_quote;

			current_index++;
		}

		// Eğer kelimenin başlangıç indeksi input uzunluğunu geçtiyse boş sonlandır
		if (start_index >= (int)ft_strlen(input))
			substrings[substr_index++] = NULL;
		else
			// Kelimeyi substring olarak al ve dizimize ekle
			substrings[substr_index++] = grbg_substr(shell, input, start_index, current_index - start_index);
	}

	return substrings;
}


/* Split the input read from the readline and put it into
	char		**commands; from the original shell struct
*/
char	**split_input(char *str, t_shell *shell)
{
	// str = boşlukları düzenlenmiş yeni string
	char	**arr;
	int		word_count;

	if (!str)
		return (NULL);
	if (ft_isspace(*str))
		str++;
	word_count = ft_count_words_ms(str, " ", 0, 0);
	if (word_count == -1)
	{
		printf("Syntax error: unclosed quote in argument\n");
		return (NULL);
	}
	//arr = komut komut ayrıldıktan sonraki dizimiz şu an sadece kelime sayısına göre yer açtık
	arr = get_grbg(shell, word_count + 1, sizeof(char *));
	if (!arr)
		return (NULL); 
	arr = ft_create_substrs(shell, arr, str, " ");
	arr[word_count] = NULL;
	return (arr);
}
