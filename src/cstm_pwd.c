/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cstm_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 13:32:03 by denizozd          #+#    #+#             */
/*   Updated: 2025/08/28 11:45:24 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	cstm_pwd(t_cmddat *cmd_data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	collect_grbg(cmd_data->shell, cwd);
	if (!cwd)
		return (1);
	ft_putstr_fd(cwd, cmd_data->outfile);
	ft_putstr_fd("\n", cmd_data->outfile);
	return (0);
}
