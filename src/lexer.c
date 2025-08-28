/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:22:52 by ecarlier          #+#    #+#             */
/*   Updated: 2025/08/28 11:48:00 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	is_only_space(const char *str)
{
	while (*str != '\0')
	{
		if (!ft_isspace(*str))
			return (0);
		str++;
	}
	return (1);
}
//is_only_space alternatifi /////////////////////////////////

/* static int is_only_space(const char *str)
{
    int only_space = 1;
    while (*str && only_space)
        if (!ft_isspace(*str++))
            only_space = 0;
    return only_space;
} */


int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}



void	lexer(t_shell *shell) // kullanıcının komut satırına yazdığı input’u okuyup işleyecek.
{
	//input stringe malloc ile yer açmıyoruz çünkü readline bunu otomatik yapıyor.
	shell->input_string = readline("minishell> ");//kullanıcı enter'a basana kadar input_string'e yazmaya devam edecek.
	//readline() fonksiyonu standart input (stdin) üzerinden klavye girişini karakter karakter takip eder ve kullanıcı 
	//Enter’a bastığında (\n karakteri geldiğinde) okuma işlemini bitirir
	collect_grbg(shell, shell->input_string);
	signals_non_interactive();// bu satırın interaktif olması gerekmez mi????
/* 	signals_non_interactive(); adı "shell interaktif değil" demiyor, sadece “şu an readline aktif değil,
	farklı bir moddayız, sinyalleri farklı ele al” demek istiyor */
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
	{
		shell->stop = 1;
	}
	if (shell->stop != 1)
		shell->commands_array = expander(shell);
}
