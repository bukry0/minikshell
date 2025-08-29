/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manuel_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:53:36 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 02:54:53 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	manuel_pwd(t_cmddat *cmd_data)
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
