/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 08:46:22 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 15:13:44 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*add_to_str(t_shell *shell, char **str, char *add)
{
	char	*new;

	if (!add)
	{
		new = grbg_strdup(shell, *str);
		return (new);
	}
	if (!str || !*str)
	{
		new = grbg_strdup(shell, add);
		return (new);
	}
	new = grbg_strjoin(shell, *str, add);
	return (new);
}

size_t	get_len_arr(char **array)
{
	size_t	i;

	i = 0;
	if (array == NULL)
		return (0);
	while (array[i])
	{
		i++;
	}
	return (i);
}

char	**del_str(char **array, int pos, int count)
{
	int	i;
	int	buff;

	i = 0;
	while (i++ < count)
	{
		buff = pos;
		while (array[++buff])
			array[buff - 1] = array[buff];
		array[buff - 1] = NULL;
	}
	return (array);
}

char	**add_str_to_arr(t_shell *shell, char **arr, char *str)
{
	char	**new;
	int		l;

	l = get_len_arr(arr);
	new = get_grbg(shell, l + 2, sizeof(char *));
	if (!new)
		return (NULL);
	new[l] = grbg_strdup(shell, str);
	l--;
	while (l >= 0)
	{
		new[l] = grbg_strdup(shell, arr[l]);
		l--;
	}
	return (new);
}
