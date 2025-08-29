/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_spaces_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 01:17:16 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 15:06:07 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	special_len(char *str, int count, size_t i)
{
	while (str[i] && str[i + 1])
	{
		if (str[i + 1] && ((str[i] == '>' && str[i + 1] == '>')
				|| (str[i] == '<' && str[i + 1] == '<')))
		{
			if (i > 0 && str[i - 1] != ' ')
				count++;
			if (str[i + 2] && str[i + 2] != ' ')
				count++;
			i += 1;
		}
		else if (str[i] == '>' || str[i] == '<' || str[i] == '|')
		{
			if (i > 0 && str[i - 1] != ' ')
				count++;
			if (str[i + 1] && str[i + 1] != ' ')
				count++;
			i++;
		}
		i++;
	}
	count++;
	return (count);
}

char	*allocate_mem(t_shell *shell, char *str)
{
	char	*new_str;
	int		len_str;

	len_str = 0;
	new_str = NULL;
	len_str = ft_strlen(str) + special_len(str, 0, 0) + 1;
	new_str = (char *)get_grbg(shell, len_str, sizeof(char));
	if (!new_str)
		return (NULL);
	return (new_str);
}

int	check_in_quote(char *str, int index)
{
	int	in_single;
	int	in_double;
	int	i;

	in_single = 0;
	in_double = 0;
	i = 0;
	while (i <= index && str[i])
	{
		if (!in_double && str[i] == '\'')
			in_single = !in_single;
		if (!in_single && str[i] == '\"')
			in_double = !in_double;
		i++;
	}	
	return (in_single || in_double);
}
