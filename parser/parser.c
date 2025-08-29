/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:34:24 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 13:18:21 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parse_remaining(t_shell *shell)
{
	int	type;

	type = 0;
	check_token(shell);
	if (shell->stop == 0)
		handle_redir(shell, type);
	if (shell->cmd_list != NULL)
		add_last_cmd_to_envp(shell);
	trim_quotes(shell);
}

int	count_segment_length(t_shell *shell, int start_index)
{
	int	length;

	length = 0;
	while (shell->commands_array[start_index]
		&&shell->commands_array[start_index][0] != '|')
	{
		start_index++;
		length++;
	}
	return (length);
}

void	create_cmd_node(t_shell *shell, int start_index, int segment_length)
{
	t_cmddat	*cmd_data;	

	cmd_data = init_struct_cmd(shell);
	if (!cmd_data)
		return ;
	cmd_data->parsed_command = fill_arr(shell, shell->commands_array,
			start_index, segment_length);
	cmd_data->full_path = indicate_path_cmds(cmd_data, shell->envp);
	collect_grbg(shell, cmd_data->full_path);
	add_node_to_list(shell, &(shell->cmd_list), cmd_data);
}

void	parser(t_shell *shell)
{
	int	index;
	int	segment_length;

	index = 0;
	check_last_char(shell);
	while (shell && shell->commands_array && shell->commands_array[index]
		&& !shell->stop)
	{
		segment_length = count_segment_length(shell, index);
		create_cmd_node(shell, index, segment_length);
		index += segment_length;
		if (shell->commands_array[index] == NULL)
			break ;
		index++;
	}	
	parse_remaining(shell);
}
