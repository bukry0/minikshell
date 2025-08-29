/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 06:22:05 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 13:19:50 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_type(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (!check_in_quote(str, i))
		{
			if (str[i] == '<' && str[i + 1] && str[i + 1] == '<')
				return (2);
			if (str[i] == '<')
				return (1);
			if (str[i] == '>' && str[i + 1] && str[i + 1] == '>')
				return (4);
			if (str[i] == '>')
				return (3);
		}
	}
	return (0);
}

void	handle_redir(t_shell *ptr, int type)
{
	int			i;
	t_node		*current_node;
	t_cmddat	*cmd_data;

	current_node = ptr->cmd_list;
	while (current_node != NULL)
	{
		cmd_data = current_node->data;
		if (cmd_data)
		{
			i = -1;
			while (cmd_data->parsed_command[++i])
			{
				type = get_type(cmd_data->parsed_command[i]);
				if (type < 5 && type > 0)
				{
					open_fd_redir(ptr, cmd_data, i, type);
					cmd_data->parsed_command = del_str(cmd_data->parsed_command,
							i, 2);
					i -= 1;
				}
			}
		}
		current_node = current_node->next;
	}
}

int	open_file(t_cmddat *cmd_struct, int redir_index,
	int open_flags, int permission_mode)
{
	int		*target_file;
	char	*filename;

	target_file = &(cmd_struct->infile);
	if (*target_file > 1)
	{
		if (close(*target_file) == -1)
			printf("Error while attempting to close a file\n");
	}
	filename = cmd_struct->parsed_command[redir_index + 1];
	if (!filename)
		return (syntax_error(NULL, filename), 1);
	if (permission_mode != 0)
		*target_file = open(filename, open_flags, permission_mode);
	else
		*target_file = open(filename, open_flags);
	if (*target_file == -1)
	{
		g_exitstatus = 1;
		print_err_msg(filename, "No such file or directory");
		return (1);
	}
	return (0);
}

int	get_flags(int type)
{
	if (type == 1 || type == 2)
		return (O_RDONLY);
	if (type == 3)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	if (type == 4)
		return (O_WRONLY | O_CREAT | O_APPEND);
	return (0);
}

int	get_permission_mode(int type)
{
	if (type == 3 || type == 4)
		return (0644);
	return (0);
}
