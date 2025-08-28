/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 13:54:59 by ecarlier          #+#    #+#             */
/*   Updated: 2025/08/28 11:49:33 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	syntax_error(t_shell *shell, char *token)
{
	if (shell)
		shell->stop = 1;
	if (!token)
		print_err_msg(NULL, "syntax error near unexpected token `newline'");
	else
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(token, 2);
		ft_putstr_fd("'\n", 2);
	}
	return (0);
}

/*@deniz you commented this, I put it here for norm :
//replace with below print_err_msg_lng if possible */
void	print_err_msg(char *cmd, char *msg)
{
	if (!msg)
		return ;
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
}

/*@deniz you commented this, I put it here for norm :
 different -> compare all err msgs with bash
*/
int	print_err_msg_lng(char *cmd, char *msg, char *arg)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
		ft_putstr_fd(cmd, 2);
	if (arg)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(arg, 2);
	}
	if (msg)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(msg, 2);
	}
	ft_putstr_fd("\n", 2);
	return (1);
}

char	*add_to_str(t_shell *shell, char **str, char *add)
{
	char	*new;

	if (!add)
	{
		new = grbg_strdup(shell, *str);
		return (new);
	}
	if (!str || !*str)
	{
		new = grbg_strdup(shell, add);
		return (new);
	}
	new = grbg_strjoin(shell, *str, add);
	return (new);
}
