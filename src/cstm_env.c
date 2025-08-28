/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cstm_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 13:51:04 by denizozd          #+#    #+#             */
/*   Updated: 2025/08/28 11:44:40 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	cstm_env(t_cmddat *cmd_data)
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
