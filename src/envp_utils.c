/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:45:17 by ecarlier          #+#    #+#             */
/*   Updated: 2025/08/28 11:53:06 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* no garbage collection for any functions in this file as they do not leak */

char	*get_path_cmds(t_cmddat *cmd, char **ev)
{
	char	*path;
	int		i;

	i = 0;
	if (get_builtin_nbr(cmd) != 0)
		return (NULL);
	if (!access(cmd->parsed_command[0], 1))
		return (ft_strdup(cmd->parsed_command[0]));
	while (cmd->parsed_command[i])
	{
		path = get_path(cmd->parsed_command[i], ev, 0);
		if (path)
			break ;
		i++;
	}
	return (path);
}

/* Get the full path of the command by searching through the PATH variable:
Searches through the PATH variable to find the full path of
the command (where it is executed),
returns the full path (including the command) if found, or
NULL if the command is not in any of the specified paths. */
char	*get_path(char *cmd, char **ev, size_t i)
{
	char	**all_paths;
	char	*path;
	char	*path_part;

	while (ev[i] && ft_strnstr(ev[i], "PATH", 4) == 0)
		i++;
	if (i == get_len_arr(ev))
		return (NULL);
	all_paths = ft_split(ev[i] + 5, ':');
	i = 0;
	while (all_paths[i])
	{
		path_part = ft_strjoin(all_paths[i], "/");
		path = ft_strjoin(path_part, cmd);
		free(path_part);
		if (access(path, F_OK) == 0)
		{
			free_split(all_paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_split(all_paths);
	return (NULL);
}

/* Frees the memory allocated for an array of strings such as
the one created with ft_split. */
void	free_split(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
		free(strs[i++]);
	free(strs);
}

void	add_last_cmd_to_envp(t_shell *shell)
{
	int			l;
	t_cmddat	*cmd;

	l = 0;
	if (!shell->cmd_list->data->parsed_command)
		return ;
	cmd = cstm_lstlast(shell->cmd_list)->data;
	l = get_len_arr(cmd->parsed_command);
	if (l)
		modify_envp(shell, "_", grbg_strdup(shell, cmd->parsed_command[0]));
}
