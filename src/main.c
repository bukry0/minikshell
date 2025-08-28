/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:56:55 by ecarlier          #+#    #+#             */
/*   Updated: 2025/08/28 11:48:21 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int		g_exitstatus = 0;

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
	launch_minishell(shell);
	return (0);
}
/* if (argc == 1 || (argc == 3 && ft_strcmp(argv[1], "-c") == 0))
{
	shell = ft_calloc(1, sizeof(t_shell));
	init_shell_struct(shell, envp);
	if (argc == 3)
		shell->input_string = ft_strdup(argv[2]);
	launch_minishell(shell);
}
else
{
	ft_putstr_fd("Usage: ./minishell [-c \"command\"]\n", 2);
	return (1);
} */
void	launch_minishell(t_shell *shell)
{
	while (1)
	{
		/* Kullanıcıdan yeni bir komut (girdi) bekleyeceğimiz için
		 terminal sinyallerini “interaktif mod”da ayarlamak gerekiyor. */
		signals_interactive();
		shell->stop = 0;
		lexer(shell);
		if (shell->stop == 0)
			parser(shell, 0, 0);
		if (cstm_lstsize(shell->cmd_list) > 1 && shell->stop == 0)
			pipe_infile_outfile(shell->cmd_list);
		if (!shell->stop)
			execute_cmds(shell);
		shell->cmd_list = NULL;
	}
}

/* no garbage collection for below function as it does not leak
set outfile to write end of pipe
set infile to read end of pipe */
void	pipe_infile_outfile(t_node *cmd_lst)
{
	int	*pip;

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
