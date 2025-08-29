/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_spaces.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 21:38:20 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 15:47:23 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	add_spaces_double(char *str, char **new_str, int *j, size_t *i)
{
	if ((*i) != 0 && str[(*i) - 1] != ' ')
		(*new_str)[(*j)++] = ' ';
	(*new_str)[(*j)++] = str[(*i)++];
	(*new_str)[(*j)++] = str[(*i)++];
	if (str[(*i)] != ' ' && str[(*i)] != '\0')
		(*new_str)[(*j)++] = ' ';
}

static	void	add_spaces(char *str, char **new_str, int *j, size_t *i)
{
	if ((str[(*i)] == '>' && str[(*i) + 1] == '>')
		|| (str[(*i)] == '<' && str[(*i) + 1] == '<'))
		add_spaces_double(str, &(*new_str), &(*j), &(*i));
	if (i != 0 && (str[(*i)] == '>' || str[(*i)] == '<'
			|| str[(*i)] == '|') && str[(*i) - 1] != ' ')
	{
		(*new_str)[(*j)++] = ' ';
		(*new_str)[(*j)] = str[(*i)];
	}
	if ((str[(*i)] == '>' || str[(*i)] == '<'
			|| str[(*i)] == '|') && str[(*i) + 1] != ' ')
	{
		(*new_str)[(*j)++] = str[(*i)++];
		(*new_str)[(*j)++] = ' ';
	}
	(*new_str)[(*j)++] = str[(*i)++];
	return ;
}

char	*handle_spaces(t_shell *shell, char *str, size_t i, int j)
{
	char	*new_str;
	int		in_quote;

	new_str = allocate_mem(shell, str);
	while (str[i])
	{
		in_quote = check_in_quote(str, i);
		if (!in_quote)
			add_spaces(str, &new_str, &j, &i);
		else
			new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	return (new_str);
}
