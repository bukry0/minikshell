/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cstm_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 18:25:12 by denizozd          #+#    #+#             */
/*   Updated: 2025/08/28 11:52:57 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

size_t	get_len_env(const char *s)
{
	size_t	l;

	l = 0;
	if (!s)
		return (0);
	while (s[l] && s[l] != '=')
		l++;
	return (l);
}

int	cstm_unset(t_cmddat *cmd)
{
	int		i;
	int		j;
	char	**envs;

	i = 0;
	envs = cmd->shell->envp;
	if (!envs || !cmd->parsed_command)
		return (1);
	while (envs[i])
	{
		j = 1;
		while (cmd->parsed_command[j] && ft_strcmp(cmd->parsed_command[j], "_"))
		{
			if (!ft_strncmp(envs[i], cmd->parsed_command[j], get_len_env(envs[i]))
				&& get_len_env(envs[i]) == ft_strlen(cmd->parsed_command[j]))
				del_str(envs, i, 1);
			j++;
		}
		i++;
	}
	return (0);
}
