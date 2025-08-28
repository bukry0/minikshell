/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cstm_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 13:50:56 by denizozd          #+#    #+#             */
/*   Updated: 2025/08/28 11:52:27 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	cstm_echo(t_cmddat *cmd_data)
{
	int	nl;
	int	i;

	nl = 1;
	i = 1;
	if (cmd_data == NULL)
		return (1);
	if (cmd_data->parsed_command[1] == NULL)
		return (ft_putstr_fd("\n", cmd_data->outfile), 1);
	if (!ft_strcmp(cmd_data->parsed_command[1], "-n"))
	{
		nl = 0;
		i += 1;
	}
	while (cmd_data->parsed_command[i])
	{
		ft_putstr_fd(cmd_data->parsed_command[i], cmd_data->outfile);
		i += 1;
		if (cmd_data->parsed_command[i] != NULL)
			ft_putstr_fd(" ", cmd_data->outfile);
	}
	if (nl)
		ft_putstr_fd("\n", cmd_data->outfile);
	return (0);
}
