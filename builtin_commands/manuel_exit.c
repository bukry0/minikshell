/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manuel_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:20:29 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 02:28:24 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_only_digits(char *s)
{
	size_t	i;

	i = 0;
	if (s[i] == '-' || s[i] == '+')
		i++;
	while (ft_isdigit(s[i]))
		i++;
	if (ft_strlen(s) == i)
		return (1);
	return (0);
}

int	manuel_exit(t_cmddat *cmd_data)
{
	g_exitstatus = 0;
	ft_putstr_fd("exit\n", cmd_data->outfile);
	if (get_len_arr(cmd_data->parsed_command) > 2)
	{
		print_err_msg("exit", "too many arguments");
		return (1);
	}
	if (get_len_arr(cmd_data->parsed_command) == 2
		&& !is_only_digits(cmd_data->parsed_command[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd_data->parsed_command[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		g_exitstatus = 2;
		exit_ms(g_exitstatus, cmd_data->shell);
	}
	if (cmd_data->parsed_command[1])
	{
		g_exitstatus = ft_atoi(cmd_data->parsed_command[1]);
		if (g_exitstatus < 0)
			g_exitstatus = 256 + g_exitstatus;
	}
	exit_ms(g_exitstatus, cmd_data->shell);
	return (0);
}
