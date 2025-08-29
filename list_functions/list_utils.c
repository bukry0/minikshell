/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 07:12:31 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 15:06:16 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_node_to_list(t_shell *shell, t_node **head, t_cmddat *data)
{
	t_node	*new_node;
	t_node	*current;

	new_node = get_grbg(shell, 1, sizeof(t_node));
	if (!new_node)
		return ;
	new_node->data = data;
	new_node->next = NULL;
	if (*head == NULL)
		*head = new_node;
	else
	{
		current = *head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = new_node;
	}
}

int	ft_listsize(t_node *lst)
{
	size_t	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
