/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_spaces.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 20:48:07 by ecarlier          #+#    #+#             */
/*   Updated: 2025/08/28 11:47:14 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
  Calculates and returns the special length of the string `str`,
  which represents the number of special characters that are not
  preceded or followed by a space.

  Parameters:
    - str: Pointer to the string to calculate the special length for.

  Returns:
    - The number of special characters that are not preceded or followed
	by a space in the string.
*/

/* static int is_double_redir(char a, char b)
{
    return ((a == '>' && b == '>') || (a == '<' && b == '<'));
}

static int is_single_special(char c)
{
    return (c == '>' || c == '<' || c == '|');
}

static int special_len(const char *str)
{
    int count = 0;
    size_t i = 0;

    while (str[i])
    {
        if (str[i + 1] && is_double_redir(str[i], str[i + 1]))
        {
            if (i > 0 && str[i - 1] != ' ')
                count++;
            if (str[i + 2] && str[i + 2] != ' ')
                count++;
            i += 2;
            continue;
        }
        else if (is_single_special(str[i]))
        {
            if (i > 0 && str[i - 1] != ' ')
                count++;
            if (str[i + 1] && str[i + 1] != ' ')
                count++;
            i++;
            continue;
        }
        i++;
    }
    return count + 1; // orijinal count++ mantığı
}
 */

static int	special_len(char *str, int count, size_t i)
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

static char	*allocate_mem(t_shell *shell, char *str)
{
	char	*new_str;
	int		base_len;
	int		extra_spaces;
	int		total_len;
	
	new_str = NULL;
	base_len = ft_strlen(str);
	extra_spaces = special_len(str, 0, 0);
	total_len = base_len + extra_spaces + 1;
	new_str = (char *)get_grbg(shell, total_len, sizeof(char));
	if (!new_str)
		return (NULL);
	return (new_str);
}
//DUZELTME YAPILABIILR--------------------- ------------------------- --------------------
static void	add_spaces_double(char *str, char **new_str, int *j, size_t *i)
{
	if (str[(*i) + 2] != ' ' && (*i) + 2 < ft_strlen(str))
	{
		(*new_str)[(*j)++] = str[(*i)++];
		(*new_str)[(*j)++] = str[(*i)++];
		(*new_str)[(*j)++] = ' ';
	}
	else if ((*i) == 0 || str[(*i) - 1] != ' ') // kodun en başında da boşluk olmalı mı ????????????
	{
		(*new_str)[(*j)++] = ' ';
		(*new_str)[(*j)++] = str[(*i)++];
		(*new_str)[(*j)++] = str[(*i)++];
	}
	else
	{
		(*new_str)[(*j)++] = str[(*i)++];
		(*new_str)[(*j)++] = str[(*i)++];
	}
	return ;
}

static	void	add_spaces(char *str, char **new_str, int *j, size_t *i)
{
	if ((str[(*i)] == '>' && str[(*i) + 1] == '>')
		|| (str[(*i)] == '<' && str[(*i) + 1] == '<'))
	{
		add_spaces_double(str, &(*new_str), &(*j), &(*i));
	}
	else if ((str[(*i)] == '>' || str[(*i)] == '<'
			|| str[(*i)] == '|') && str[(*i) + 1] != ' ')
	{
		(*new_str)[(*j)++] = str[(*i)++];
		(*new_str)[(*j)++] = ' ';
	}
	else if (i != 0 && (str[(*i)] == '>' || str[(*i)] == '<'
			|| str[(*i)] == '|') && str[(*i) - 1] != ' ')
	{
		(*new_str)[(*j)++] = ' ';
		(*new_str)[(*j)++] = str[(*i)++];
	}
	else
		(*new_str)[(*j)++] = str[(*i)++];
	return ;
}

char	*handle_spaces(t_shell *shell, char *str, size_t i, int j)
//str = kullanıcının input_string'i
{
	char	*new_str;
	int		sgq;
	int		dbq;

	sgq = 0; //tek tırnak içinde mi
	dbq = 0; //çift tırnak içinde mi
	new_str = allocate_mem(shell, str); // alan açma işlemini ilk önce yapıyoruz ama eğer redir tırnak içindeyse memory leak olmaz mı?
	while (str[i])
	{
		// TIRNAK KONTROLÜ DEĞİŞİKLİĞİ////////////////////////////////////////
/* 		if(str[i] = = '\'' && !dbq)
			sgq = !sgq
		else if(str[i] == '\"' && !sgq)
			dbq = !dbq; */
		sgq = (sgq + (!dbq && str[i] == '\'')) % 2; // ifadesi bool (doğru veya yanlış) döner.
		dbq = (dbq + (!sgq && str[i] == '\"')) % 2;
		if (!sgq && !dbq)
		{
			add_spaces(str, &new_str, &j, &i);
		}
		else
			new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	return (new_str);
}
