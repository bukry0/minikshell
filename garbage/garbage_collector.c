/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 05:22:31 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 15:16:13 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*get_grbg(t_shell *shell, size_t nmemb, size_t size)
{
	void	*new;

	new = ft_calloc(nmemb, size);
	if (!new)
	{
		ft_putstr_fd("memory allocation error\n", 2);
		g_exitstatus = 1;
		return (NULL);
	}
	collect_grbg(shell, new);
	return (new);
}

void	collect_grbg(t_shell *shell, void *new)
{
	t_grbg	*node;
	t_grbg	*tmp;
	t_grbg	**head;

	head = &(shell->grbg_lst);
	node = ft_calloc(1, sizeof(t_grbg));
	if (!node)
	{
		ft_putstr_fd("memory allocation error\n", 2);
		g_exitstatus = 1;
		return ;
	}
	node->ptr = new;
	node->next = NULL;
	if (!(*head))
	{
		*head = node;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
	return ;
}

void	free_grbg(t_grbg *head)
{
	t_grbg	*curr;
	t_grbg	*prev;

	curr = head;
	while (curr)
	{
		if (curr->ptr)
			free(curr->ptr);
		prev = curr;
		if (curr->next)
			curr = curr->next;
		else
		{
			free(curr);
			return ;
		}
		free(prev);
	}
}
