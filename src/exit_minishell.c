/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 15:19:36 by ecarlier          #+#    #+#             */
/*   Updated: 2025/08/28 11:53:22 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*free and exit*/
void	exit_ms(int g_exitstatus, t_shell *shell)
{
	free_all(shell);
	exit(g_exitstatus);
}

void	free_all(t_shell *shell)
{
	free_grbg(shell->grbg_lst);
	rl_clear_history(); // readline kütüphanesinin tuttuğu komut geçmişini temizler
	if (shell)
		free(shell);
}

void	free_node_list(t_node *head)
{
	t_node	*current;
	t_node	*temp;

	current = head;
	while (current)
	{
		temp = current;
		current = current->next;
		if (temp && temp->data)
		{
			if (temp->data->parsed_command)
				free(temp->data->parsed_command);
			if (temp->data->full_path)
				free(temp->data->full_path);
			free(temp->data);
		}
		free(temp);
	}
}
