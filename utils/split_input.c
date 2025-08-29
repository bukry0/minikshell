/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 08:00:36 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 15:13:36 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_count_words_ms(char *str, char *sep, int count, int i)
{
	while (str[i] != '\0')
	{
		if (!ft_strchr(sep, str[i]))
		{
			count++;
			while ((!ft_strchr(sep, str[i]) || check_in_quote(str, i))
				&& str[i] != '\0')
				i++;
			if (check_in_quote(str, i))
				return (-1);
		}
		else
			i++;
	}
	return (count);
}

char	**ft_create_substrs(t_shell *shell, char **substrings,
			char *input, char *separators)
{
	int	current_index;
	int	start_index;
	int	substr_index;

	current_index = 0;
	start_index = 0;
	substr_index = 0;
	while (input[current_index])
	{
		while (ft_strchr(separators, input[current_index])
			&& input[current_index] != '\0')
			current_index++;
		start_index = current_index;
		while ((!ft_strchr(separators, input[current_index])
				|| check_in_quote(input, current_index))
			&& input[current_index])
			current_index++;
		if (start_index >= (int)ft_strlen(input))
			substrings[substr_index++] = NULL;
		else
			substrings[substr_index++] = grbg_substr(shell, input, start_index,
					current_index - start_index);
	}
	return (substrings);
}

char	**split_input(char *str, t_shell *shell)
{
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
	arr = get_grbg(shell, word_count + 1, sizeof(char *));
	if (!arr)
		return (NULL);
	arr = ft_create_substrs(shell, arr, str, " ");
	arr[word_count] = NULL;
	return (arr);
}
