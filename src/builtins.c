/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 10:34:24 by denizozd          #+#    #+#             */
/*   Updated: 2025/08/28 11:52:08 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	get_builtin_nbr(t_cmddat *cmd)
{
	if (cmd->parsed_command == NULL || cmd->parsed_command[0] == NULL)
		return (0);
	else if (!ft_strcmp(cmd->parsed_command[0], "echo"))
		return (1);
	else if (!ft_strcmp(cmd->parsed_command[0], "cd"))
		return (2);
	if (!ft_strcmp(cmd->parsed_command[0], "pwd"))
		return (3);
	else if (!ft_strcmp(cmd->parsed_command[0], "export"))
		return (4);
	else if (!ft_strcmp(cmd->parsed_command[0], "unset"))
		return (5);
	else if (!ft_strcmp(cmd->parsed_command[0], "env"))
		return (6);
	else if (!ft_strcmp(cmd->parsed_command[0], "exit"))
		return (7);
	return (0);
}

/* g_exitstatus 127 when command to execute could not be found */
int	execute_builtin(t_cmddat *cmd, int n, int forked)
{
	int	r;

	if (n == 1)
		r = cstm_echo(cmd);
	else if (n == 2)
		r = cstm_cd(cmd);
	else if (n == 3)
		r = cstm_pwd(cmd);
	else if (n == 4)
		r = cstm_export(cmd);
	else if (n == 5)
		r = cstm_unset(cmd);
	else if (n == 6)
		r = cstm_env(cmd);
	else if (n == 7)
		r = cstm_exit(cmd);
	else
		r = 127;
	if (forked)
	{
		cstm_lstiter(cmd->shell->cmd_list, cls_fds);
		exit_ms(r, cmd->shell);
	}
	return (r);
}
