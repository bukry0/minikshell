/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 03:53:33 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 20:26:22 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_env_value(t_shell *shell, char *str, t_exp *exp, char *sub_str)
{
	if (!exp->s_quote && str[exp->current_index] == '$'
		&& str[exp->current_index + 1]
		&& str[exp->current_index + 1] != ' ')
	{
		if (str[exp->current_index + 1] == '?')
			sub_str = handle_g_exitstatus(shell, exp->current_index,
					str, sub_str);
		else
		{
			exp->variable_length = get_len_var(str, exp->current_index + 1);
			sub_str = create_sub_var(str, exp->current_index,
					shell->envp, exp->variable_length);
			collect_grbg(shell, sub_str);
			if (!sub_str)
			{
				sub_str = "";
				return (NULL);
			}
		}
		str = sub_str;
		exp->current_index = 0;
		return (sub_str);
	}
	return (str);
}

char	*handle_expansion(t_shell *shell, char *str, t_exp *exp, char *sub_str)
{
	char	*result;

	while (str[exp->current_index])
	{
		exp->s_quote = (exp->s_quote + (!exp->d_quote
					&& str[exp->current_index] == '\'')) % 2;
		exp->d_quote = (exp->d_quote
				+ (!exp->s_quote && str[exp->current_index] == '\"')) % 2;
		result = find_env_value(shell, str, exp, sub_str);
		if (!result)
			break ;
		if (result != str)
		{
			str = result;
			continue ;
		}
		exp->current_index++;
	}
	return (str);
}

char	*handle_g_exitstatus(t_shell *shell, int i, char *str, char *sub_str)
{
	char	*exit_code_str;

	exit_code_str = NULL;
	if (str[i + 1] == '?')
	{
		exit_code_str = grbg_itoa(shell, g_exitstatus);
		if (!exit_code_str)
			return (NULL);
		sub_str = create_sub(str, i, exit_code_str, ft_strlen(exit_code_str));
		collect_grbg(shell, sub_str);
	}
	return (sub_str);
}

char	*expand_var(t_shell *shell, char *str, int i)
{
	t_exp	*exp;
	char	*sub_str;

	sub_str = NULL;
	exp = get_grbg(shell, 1, sizeof(t_exp));
	if (!exp)
		return (NULL);
	exp->s_quote = 0;
	exp->d_quote = 0;
	exp->current_index = i;
	exp->variable_length = 0;
	if (ft_strcmp(str, "$") && ft_strlen(str) == 3)
		return (str);
	sub_str = handle_expansion(shell, str, exp, sub_str);
	if (sub_str)
		return (sub_str);
	else
		return (str);
}

char	**expander(t_shell *shell)
{
	int		i;
	char	**temp;

	temp = get_grbg(shell, get_len_arr(shell->commands_array) + 1,
			sizeof(char *));
	if (!temp)
		return (NULL);
	i = -1;
	while (shell->commands_array[++i])
		temp[i] = expand_var(shell, shell->commands_array[i], 0);
	temp[i] = NULL;
	i = 0;
	while (temp[i] != NULL)
	{
		if (ft_strcmp(temp[i], "") == 0)
		{
			del_str(temp, i, 1);
			i = 0;
		}
		else
			i++;
	}
	return (temp);
}
