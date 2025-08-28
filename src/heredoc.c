/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 15:25:27 by ecarlier          #+#    #+#             */
/*   Updated: 2025/08/28 11:53:49 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int is_valid_delimiter(char *lim)//alfanümerik kontrolü bu fonksiyonla yapılacak //EKLEME
{
    size_t i = 0;
    while (lim[i])
    {
        if (!ft_isalnum(lim[i]))
            return (0);
        i++;
    }
    return (1);
}

void	launch_heredoc(t_shell *shell, t_cmddat *cmd, int i)
{
	char	*lim;
	
	lim = cmd->parsed_command[i + 1];
	if (!is_valid_delimiter(lim))
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


int	get_heredoc(t_shell *shell, char *lim)
{
	char	*content;
	char	*line;

	content = NULL;
	line = NULL;
	g_exitstatus = 0;//exit statusun önceki değerini korumak gerekli mi ?
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
		line = expand_var(shell, line, shell->envp, 0);
		content = add_to_str(shell, &content, line);
		content = add_to_str(shell, &content, "\n");
	}
	return (pipe_heredoc(content));
}

int	pipe_heredoc(char *content)
{
	int	pip[2];

	if (g_exitstatus)
		return (0);
	if (!pipe(pip))
	{
		ft_putstr_fd(content, pip[1]);
		close(pip[1]);
		return (pip[0]);
	}
	return (0);
}
