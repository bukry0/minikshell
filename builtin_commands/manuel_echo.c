/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manuel_echo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:12:57 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 02:21:27 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	manuel_echo(t_cmddat *cmd_data)
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
