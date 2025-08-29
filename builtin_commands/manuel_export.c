/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manuel_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:33:59 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 15:30:52 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	manuel_export(t_cmddat *cmd)
{
	int	r;
	int	i;
	int	id_len;

	r = 0;
	i = 1;
	id_len = 0;
	if (get_len_arr(cmd->parsed_command) == 1)
		return (print_export(cmd));
	if (!cmd->shell->envp && get_len_arr(cmd->parsed_command) > 1
		&& get_len_id(cmd->shell, cmd->parsed_command[i], 0))
		cmd->shell->envp = add_str_to_arr(cmd->shell, cmd->shell->envp,
				cmd->parsed_command[i++]);
	while (cmd->parsed_command[i])
	{
		id_len = get_len_id(cmd->shell, cmd->parsed_command[i], 1);
		if (id_len)
			scan_envp(cmd, cmd->parsed_command[i], id_len);
		else
			r = 1;
		i++;
	}
	return (r);
}

int	print_export(t_cmddat *cmd)
{
	int	i;

	i = 0;
	while (cmd->shell->envp && cmd->shell->envp[i])
	{
		if (ft_strncmp(cmd->shell->envp[i], "_=", 2))
			print_line_export(cmd, i);
		i++;
	}
	return (0);
}

void	print_line_export(t_cmddat *cmd, int i)
{
	size_t	l;

	l = get_len_id(cmd->shell, cmd->shell->envp[i], 0);
	ft_putstr_fd("declare -x ", cmd->outfile);
	if (l && l != ft_strlen(cmd->shell->envp[i]))
	{
		write(cmd->outfile, cmd->shell->envp[i], l);
		ft_putstr_fd("=\"", cmd->outfile);
		ft_putstr_fd(cmd->shell->envp[i] + l + 1, cmd->outfile);
		ft_putstr_fd("\"", cmd->outfile);
	}
	else if (l)
		write(cmd->outfile, cmd->shell->envp[i], l);
	else
		ft_putstr_fd(cmd->shell->envp[i], cmd->outfile);
	ft_putstr_fd("\n", cmd->outfile);
}

int	get_len_id(t_shell *shell, char *str, int msg)
{
	int		i;
	int		e;
	char	*tmp;

	i = 0;
	e = 0;
	while (str[i] && str[i] != '=')
	{
		if (ft_isdigit(str[0]))
			e = 1;
		if (!ft_isalnum(str[i]) && str[i] != '_')
			e = 1;
		i++;
	}
	if (e || !i)
	{
		if (msg)
		{
			tmp = grbg_strjoin(shell, "'", str);
			tmp = add_to_str(shell, &tmp, "'");
			print_err_msg_lng("export", "not a valid identifier", tmp);
		}
		i = 0;
	}
	return (i);
}

int	scan_envp(t_cmddat *cmd, char *str, int id_len)
{
	size_t	i;
	int		ev_id_len;

	i = -1;
	while (cmd->shell->envp[++i])
	{
		ev_id_len = get_len_id(cmd->shell, cmd->shell->envp[i], 0);
		if (ev_id_len == id_len && !ft_strncmp(cmd->shell->envp[i], str,
				id_len))
		{
			if (ft_strchr(str, '='))
				modify_envp(cmd->shell,
					grbg_substr(cmd->shell, cmd->shell->envp[i], 0, ev_id_len),
					grbg_strdup(cmd->shell, str + ev_id_len + 1));
			break ;
		}
		else if (i == get_len_arr(cmd->shell->envp) - 1)
		{
			cmd->shell->envp = add_str_to_arr(cmd->shell, cmd->shell->envp,
					str);
			break ;
		}
	}
	return (0);
}
