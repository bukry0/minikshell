/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 20:28:58 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 15:41:34 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_only_space(char *str)
{
	while (*str != '\0')
	{
		if (!isspace(*str))
			return (0);
		str++;
	}
	return (1);
}

void	lexer(t_shell *shell)
{
	shell->input_string = readline("minikshell$ ");
	collect_grbg(shell, shell->input_string);
	signals_non_interactive();
	add_history(shell->input_string);
	if (shell->input_string == NULL)
	{
		shell->stop = 1;
		exit_ms(0, shell);
		return ;
	}
	if (ft_strlen(shell->input_string) <= 0
		|| is_only_space(shell->input_string))
	{
		shell->stop = 1;
		return ;
	}
	shell->input_string = handle_spaces(shell, shell->input_string, 0, 0);
	shell->commands_array = split_input(shell->input_string, shell);
	if (shell->commands_array == NULL)
		shell->stop = 1;
	if (shell->stop != 1)
		shell->commands_array = expander(shell);
}
