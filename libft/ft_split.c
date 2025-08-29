/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 19:18:44 by bcili             #+#    #+#             */
/*   Updated: 2024/12/04 14:23:14 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_word(const char *s, char c, char ***res, int *i)
{
	int	count_word;
	int	flag;

	count_word = 0;
	flag = 0;
	*i = 0;
	while (*s)
	{
		if (*s != c && !flag)
		{
			flag = 1;
			count_word++;
		}
		else if (*s == c)
			flag = 0;
		s++;
	}
	*res = malloc(sizeof(char *) * (count_word + 1));
	return (count_word);
}

static int	free_node(char **str, int index)
{
	int	i;

	i = 0;
	if (!str[index - 1])
	{
		while (str[i])
		{
			free(str[i]);
			i++;
		}
		free(str);
		return (0);
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		i;
	int		j;
	int		len;

	if (!s)
		return (0);
	len = ft_count_word(s, c, &result, &i);
	if (!result)
		return (0);
	while (i < len)
	{
		while (*s == c)
			s++;
		j = 0;
		while (s[j] != c && s[j])
			j++;
		result[i++] = ft_substr(s, 0, j);
		if (free_node(result, i) == 0)
			return (0);
		s += j;
	}
	result[i] = 0;
	return (result);
}
