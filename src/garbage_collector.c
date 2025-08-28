/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 17:50:33 by denizozd          #+#    #+#             */
/*   Updated: 2025/08/28 11:46:30 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"



/*	nmemb and size of new memory space (same as for ft_calloc) */
void	*get_grbg(t_shell *shell, size_t nmemb, size_t size) //envin uzulğu ve sizeof
{
/* 	alanın adresini, otomatik olarak bir "çöp toplayıcı" 
	(garbage collector) listesine ekler. Böylece, programın sonunda veya ihtiyaç duyduğunda,
	 bu listede tutulan tüm bellekler kolayca serbest bırakılabilir. */
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
	//bu fonksiyonda promp parametresinin tek amacı collect_grb fonksiyonuna göndermek
}

/*	can also be used without get_grbg to collect malloc'ed space */
void	collect_grbg(t_shell *shell, void *new)
{
	t_grbg	*node;
	t_grbg	*tmp;
	t_grbg	**head;
/* shell->grbg_lst bir pointer'dır (bağlı listenin başını tutar) 
& operatörü ile, bu pointer'ın adresini alıyorsun.
Yani, head artık shell->grbg_lst pointer'ının adresini tutar.*/
	head = &(shell->grbg_lst);
/* fonksiyonun devamında, listenin başı değişirse (*head = node; gibi),
doğrudan shell->grbg_lst’yi güncelleyebiliyorsun.
Yani listenin başına kolayca ekleme yapabilmek için bu yöntem kullanılır. */
	node = ft_calloc(1, sizeof(t_grbg));
	if (!node)
	{
		ft_putstr_fd("memory allocation error\n", 2);
		g_exitstatus = 1;
		return ;
	}
	node->ptr = new; //malloc/calloc edilmiş alanın adresini çöp listesine ekler. ile düğümün içine senin string adresini kaydediyo
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
/* head üzerinden yapılan her değişiklik (örneğin *head = node;)
otomatik olarak shell->grbg_lst üzerinde de gerçekleşir. */
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
