/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 09:44:46 by denizozd          #+#    #+#             */
/*   Updated: 2025/08/28 11:58:22 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_executable(t_cmddat *cmd_data)
{
	int	builtin;

	builtin = get_builtin_nbr(cmd_data);
	if (cmd_data->file_open_flag > 0)
	{
		g_exitstatus = cmd_data->file_open_flag;
		return (0);
	}
	if (!builtin && !cmd_data->full_path)
	{
		if (cmd_data->parsed_command[0])
			print_err_msg_lng(cmd_data->parsed_command[0], "command not found", NULL);
		g_exitstatus = 127;
		return (0);
	}
	else if (!builtin && cmd_data->full_path && (!access(cmd_data->full_path,
				F_OK) && access(cmd_data->full_path, X_OK) == -1))
	{
		if (cmd_data->parsed_command[0])
			print_err_msg_lng(cmd_data->parsed_command[0], "permission denied", NULL);
		g_exitstatus = 126;
		return (0);
	}
	return (1);
}

/*	Wait for first child process to terminate;
	if it exits normally (not by a signal),
	update temp_exitcode with the exit status of the child process.
	Wait for all child processes to terminate.
	If exitstatus changed while waiting, update existatus.
	F_OK checks that file exists, X_OK checks that file executable.	*/
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

/* exitstatus is 2 when misuse of shell builtins -> e.g.,
	ls: cannot access command-line argument */
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
		execve(cmd_data->full_path, cmd_data->parsed_command, cmd_data->shell->envp);
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
