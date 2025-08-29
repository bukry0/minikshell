/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 20:34:14 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 20:41:15 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_shell_struct(t_shell *shell, char **envp)
{
	shell->input_string = NULL;
	shell->commands_array = NULL;
	shell->cmd_list = NULL;
	shell->pid = -1;
	shell->stop = 0;
	shell->grbg_lst = NULL;
	init_env(shell, envp);
}

int	init_env(t_shell *shell, char **env)
{
	int	i;

	i = 0;
	shell->envp = get_grbg(shell, get_len_arr(env) + 1, sizeof(char *));
	if (!shell->envp)
	{
		printf("Error! About Environment Allocation.\n");
		return (0);
	}
	while (env[i])
	{
		shell->envp[i] = grbg_strdup(shell, env[i]);
		if (!shell->envp[i])
		{
			printf("Error! About Environment Allocation.\n");
			return (0);
		}
		i++;
	}
	shell->envp[i] = NULL;
	return (1);
}

t_cmddat	*init_struct_cmd(t_shell *shell)
{
	t_cmddat	*ptr;

	ptr = get_grbg(shell, 1, sizeof(t_cmddat));
	if (!ptr)
		return (NULL);
	ptr->parsed_command = NULL;
	ptr->infile = STDIN_FILENO;
	ptr->outfile = STDOUT_FILENO;
	ptr->shell = shell;
	ptr->file_open_flag = 0;
	return (ptr);
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
