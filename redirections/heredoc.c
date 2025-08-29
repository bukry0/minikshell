/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 06:10:32 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 15:17:59 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	launch_heredoc(t_shell *shell, t_cmddat *cmd, int i)
{
	char	*lim;
	size_t	j;

	lim = cmd->parsed_command[i + 1];
	j = 0;
	while (ft_isalnum(lim[j]))
		j++;
	if (j != ft_strlen(lim))
	{
		ft_putstr_fd("minishell: input error: delimiter must ", 2);
		ft_putstr_fd("contain only alphanumeric characters\n", 2);
		shell->stop = 1;
		return ;
	}
	cmd->infile = get_heredoc(shell, lim);
	if (g_exitstatus == 1)
		shell->stop = 1;
}

int	open_fd_redir(t_shell *shell, t_cmddat *cmd_struct, int i, int type)
{
	int	open_flags;
	int	permission_mode;

	open_flags = get_flags(type);
	permission_mode = get_permission_mode(type);

	trim_quotes(shell);

	if (type == 1)
		cmd_struct->file_open_flag = open_file(cmd_struct,
				i, open_flags, permission_mode);
	else if (type == 2)
		launch_heredoc(shell, cmd_struct, i);
	else
		cmd_struct->file_open_flag = open_file(cmd_struct,
				i, open_flags, permission_mode);

	return (0);
}

int	get_heredoc(t_shell *shell, char *lim)
{
	char	*content;
	char	*line;

	content = NULL;
	line = NULL;
	g_exitstatus = 0;
	while (1)
	{
		signals_interactive();
		line = readline("> ");
		collect_grbg(shell, line);
		signals_non_interactive();
		if (!line)
		{
			print_err_msg("warning", "here-document delimited by end-of-file");
			break ;
		}
		if (!ft_strncmp(line, lim, ft_strlen(line))
			&& ft_strlen(line) == ft_strlen(lim))
			break ;
		line = expand_var(shell, line, 0);
		content = add_to_str(shell, &content, line);
		content = add_to_str(shell, &content, "\n");
	}
	return (pipe_heredoc(shell, content));
}

int	pipe_heredoc(t_shell *shell, char *content)
{
	int	pip[2];

	if (g_exitstatus || !content)
	{
		shell->stop = 1;
		return (0);
	}
	if (!pipe(pip))
	{
		ft_putstr_fd(content, pip[1]);
		close(pip[1]);
		return (pip[0]);
	}
	return (0);
}
