/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 03:34:07 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 03:40:15 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	wait_update_exitstatus(t_shell *shell)
{
	int			tmp_exitstatus;
	int			child_status;
	t_cmddat	*last;

	tmp_exitstatus = -1;
	last = cstm_lstlast(shell->cmd_list)->data;
	if (waitpid(shell->pid, &child_status, 0) != -1 && WIFEXITED(child_status))
		tmp_exitstatus = WEXITSTATUS(child_status);
	while (waitpid(-1, &child_status, 0) != -1)
		continue ;
	if (tmp_exitstatus != -1)
		g_exitstatus = tmp_exitstatus;
	if (last->file_open_flag != 0)
		g_exitstatus = last->file_open_flag;
	else if (!last->full_path && !get_builtin_nbr(last))
		g_exitstatus = 127;
	else if (last->full_path && !get_builtin_nbr(last)
		&& (!access(last->full_path, F_OK) && access(last->full_path,
				X_OK < 0)))
		g_exitstatus = 126;
	return ;
}

void	cls_fds(void *content)
{
	t_cmddat	*cmd_data;

	cmd_data = (t_cmddat *)content;
	if (cmd_data->infile != -1 && cmd_data->infile != 0)
		close(cmd_data->infile);
	if (cmd_data->outfile != -1 && cmd_data->outfile != 1)
		close(cmd_data->outfile);
}

int	is_workable_path(t_cmddat *cmd_data, int builtin)
{
	if (!builtin && !cmd_data->full_path)
	{
		if (cmd_data->parsed_command[0])
			print_err_msg_lng(cmd_data->parsed_command[0],
				"command not found", NULL);
		g_exitstatus = 127;
		return (0);
	}
	else if (!builtin && cmd_data->full_path && (!access(cmd_data->full_path,
				F_OK) && access(cmd_data->full_path, X_OK) == -1))
	{
		if (cmd_data->parsed_command[0])
			print_err_msg_lng(cmd_data->parsed_command[0],
				"permission denied", NULL);
		g_exitstatus = 126;
		return (0);
	}
	return (1);
}
