/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 03:10:10 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 03:40:03 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_executable(t_cmddat *cmd_data)
{
	int	builtin;

	builtin = get_builtin_nbr(cmd_data);
	if (cmd_data->file_open_flag > 0)
	{
		g_exitstatus = cmd_data->file_open_flag;
		return (0);
	}
	if (!is_workable_path(cmd_data, builtin))
		return (0);
	return (1);
}

void	run_cmd(void *content)
{
	t_cmddat	*cmd_data;

	cmd_data = (t_cmddat *)content;
	if (cmd_data->shell->stop == 1)
		return ;
	if (!is_executable(cmd_data))
		return ;
	cmd_data->shell->pid = fork();
	if (cmd_data->shell->pid == -1)
		return ;
	if (cmd_data->shell->pid == 0)
	{
		if (get_builtin_nbr(cmd_data))
			execute_builtin(cmd_data, get_builtin_nbr(cmd_data), 1);
		dup2(cmd_data->infile, 0);
		dup2(cmd_data->outfile, 1);
		cstm_lstiter(cmd_data->shell->cmd_list, cls_fds);
		execve(cmd_data->full_path, cmd_data->parsed_command,
			cmd_data->shell->envp);
		close(0);
		close(1);
		exit_ms(2, cmd_data->shell);
	}
	return ;
}

int	execute_cmds(t_shell *shell)
{
	t_cmddat	*cmd_data;

	if (!shell->cmd_list || !shell->cmd_list->data->parsed_command[0])
		return (0);
	cmd_data = shell->cmd_list->data;
	if (cstm_lstsize(shell->cmd_list) == 1 && get_builtin_nbr(cmd_data))
	{
		g_exitstatus = execute_builtin(cmd_data, get_builtin_nbr(cmd_data), 0);
		cstm_lstiter(shell->cmd_list, cls_fds);
	}
	else
	{
		cstm_lstiter(shell->cmd_list, run_cmd);
		cstm_lstiter(shell->cmd_list, cls_fds);
		wait_update_exitstatus(shell);
	}
	return (0);
}
