/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manuel_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:55:27 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 13:26:40 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	get_len_env(char *s)
{
	size_t	l;

	l = 0;
	if (!s)
		return (0);
	while (s[l] && s[l] != '=')
		l++;
	return (l);
}

int	manuel_unset(t_cmddat *cmd)
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
			if (!ft_strncmp(envs[i], cmd->parsed_command[j],
					get_len_env(envs[i]))
				&& get_len_env(envs[i]) == ft_strlen(cmd->parsed_command[j]))
				del_str(envs, i, 1);
			j++;
		}
		i++;
	}
	return (0);
}
