/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manuel_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:16:56 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 02:21:16 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	manuel_env(t_cmddat *cmd_data)
{
	int	i;

	i = -1;
	if (cmd_data->shell->envp == NULL)
		return (1);
	while (cmd_data->shell->envp[++i])
	{
		if (ft_strchr(cmd_data->shell->envp[i], '='))
		{
			ft_putstr_fd(cmd_data->shell->envp[i], cmd_data->outfile);
			ft_putstr_fd("\n", cmd_data->outfile);
		}
	}
	return (0);
}
