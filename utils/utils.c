/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 09:08:35 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 09:12:10 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_double_pipes(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->commands_array[i])
	{
		if (shell->commands_array[i][0] == '|' && shell->commands_array[i + 1]
			&& shell->commands_array[i + 1][0] == '|')
			return (1);
		i++;
	}
	return (0);
}

int	ft_isspace(int c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}
