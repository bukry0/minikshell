/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:48:02 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 15:39:29 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exitstatus = 0;

void	start_main_loop(t_shell *shell)
{
	while (1)
	{
		signals_interactive();
		shell->stop = 0;
		lexer(shell);
		if (shell->stop == 0)
			parser(shell);
		if (cstm_lstsize(shell->cmd_list) > 1 && shell->stop == 0)
			pipe_infile_outfile(shell->cmd_list);
		if (!shell->stop)
			execute_cmds(shell);
		shell->cmd_list = NULL;
	}
}

void	pipe_infile_outfile(t_node *cmd_lst)
{
	int		*pip;

	while (cmd_lst->next != NULL)
	{
		pip = ft_calloc(2, sizeof(int));
		if (!pip)
			return ;
		if (pipe(pip) == -1)
		{
			free(pip);
			return ;
		}
		if (cmd_lst->data->outfile == 1)
			cmd_lst->data->outfile = pip[1];
		else
			close(pip[1]);
		if (cmd_lst->next->data->infile == 0)
			cmd_lst->next->data->infile = pip[0];
		else
			close(pip[0]);
		cmd_lst = cmd_lst->next;
		free(pip);
	}
	return ;
}

int	main(int argc, char *argv[], char **envp)
{
	t_shell	*shell;

	shell = NULL;
	(void)argv;
	if (argc == 1)
	{
		shell = ft_calloc(1, sizeof(t_shell));
		init_shell_struct(shell, envp);
	}
	else
	{
		printf("Error! This Program Does Not Accept Any Argument.\n");
		exit(0);
	}
	start_main_loop(shell);
	return (0);
}
