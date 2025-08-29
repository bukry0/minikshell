/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:48:02 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 20:41:08 by bcili            ###   ########.fr       */
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
