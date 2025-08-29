/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 20:14:03 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 15:14:28 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**fill_arr(t_shell *main_shell, char **shell, int i, int len)
{
	char	**temp;
	int		j;

	j = 0;
	temp = NULL;
	temp = get_grbg(main_shell, len + 1, sizeof(char *));
	if (!temp)
		return (NULL);
	while (len > 0)
	{
		temp[j] = grbg_strdup(main_shell, shell[i]);
		i++;
		j++;
		len--;
	}
	temp[j] = NULL;
	return (temp);
}

void	check_last_char(t_shell *shell)
{
	int		len_ar;
	char	last_char;

	if (check_double_pipes(shell) == 1)
	{
		shell->stop = 1;
		return ;
	}
	len_ar = get_len_arr(shell->commands_array) - 1;
	if (len_ar < 0 || shell->commands_array[len_ar] == NULL
		|| shell->commands_array[len_ar][0] == '\0')
		return ;
	last_char = shell->commands_array[len_ar][0];
	if (last_char == '|' || last_char == '<' || last_char == '>' )
	{
		shell->stop = 1;
		syntax_error(shell, shell->commands_array[len_ar]);
	}
}

void	check_token(t_shell *shell)
{
	t_node		*current_node;
	t_cmddat	*cmd_data;
	int			len_arr;
	char		last_char;

	current_node = shell->cmd_list;
	while (current_node != NULL)
	{
		cmd_data = current_node->data;
		len_arr = get_len_arr(cmd_data->parsed_command) - 1 ;
		last_char = cmd_data->parsed_command[len_arr][0];
		if (last_char == '|' || last_char == '<' || last_char == '>' )
		{
			shell->stop = 1;
			syntax_error(shell, cmd_data->parsed_command[len_arr]);
			free_node_list(shell->cmd_list);
			shell->cmd_list = NULL;
			break ;
		}
		current_node = current_node->next;
	}
}
