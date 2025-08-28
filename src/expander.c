/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 19:41:31 by ecarlier          #+#    #+#             */
/*   Updated: 2025/08/28 12:32:38 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
  Expands environment variables in the array of strings `str` using
  the environment variables `ev`.
  It creates a new array with expanded strings and returns it.

  Parameters:
    - str: Array of strings to be expanded.
    - ev:  Array of strings representing environment variables.

  Returns:
    - A new array of strings with expanded environment variables.
*/

char	**expander(t_shell *shell)
{
	int		i;
	char	**temp;

	temp = get_grbg(shell, get_len_arr(shell->commands_array) + 1, sizeof(char *));
	if (!temp)
		return (NULL);
	i = 0;
	while (shell->commands_array[i])
	{
		temp[i] = expand_var(shell, shell->commands_array[i], shell->envp, 0);
		i++;
	}
	temp[i] = NULL;
	i = 0;
	while (temp[i] != NULL) //Buradaki döngü, boş string ("") olan elemanları temizlemek için çalışıyor
	{
		if (ft_strcmp(temp[i], "") == 0)//Eğer temp[i] boşsa, yani değişken genişletildiğinde bir şey dönmediyse
		{
			del_str(temp, i, 1);//Elemanı sildikten sonra dizideki diğer elemanların indeksleri kayıyor.Bu yüzden, diziyi baştan kontrol etmek için i sıfırlanıyor
			i = 0;
		}
		else
			i++;
	}
	return (temp);
}

/*
  Expands environment variables in the string `str` using
  the environment variables `ev`.
  It modifies the original string in-place and returns it.

  Parameters:
    - str: Pointer to the string to be expanded.
    - ev:  Array of strings representing environment variables.

  Returns:
    - Pointer to the modified string with expanded environment variables.
*/
/* char	*expand_var(t_shell *shell, char *str, char **ev, int i)
{	//str = commands(tek bir komut)
	// ev = envp
	//i = 0
	
	int		q[4];
	char	*sub_str;

	(void)ev;
	sub_str = NULL;
	q[0] = 0;
	s_quote = 0;
	d_quote = 0;
	current_index = 0;
	variable_length = 0;
	q[1] = 0;
	q[2] = i;
	q[3] = 0;
	if (ft_strcmp(str, "$") && ft_strlen(str) == 3)
		return (str);
	sub_str = handle_expansion(shell, str, q, sub_str);
	if (sub_str)
		return (sub_str);
	else
		return (str);
} */
typedef struct s_exp
{
	int s_quote;         // Tek tırnak içinde mi (0 = hayır, 1 = evet)
	int d_quote;         // Çift tırnak içinde mi (0 = hayır, 1 = evet)
	int current_index;   // Str üzerinde gezinilen karakterin indeksi
	int variable_length; // Bulunan değişkenin uzunluğu
} t_exp;

char *expand_var(t_shell *shell, char *str, char **ev, int i)
//promt, bir tane komut, env, i = 0;
{
	t_exp *exp;
	char *sub_str;

	(void)ev;
	sub_str = NULL;
	exp = get_grbg(shell, 1, sizeof(t_exp)); // malloc + çöp listesine ekleme
	if (!exp)
		return NULL;

	exp->s_quote= 0;
	exp->d_quote = 0;
	exp->current_index = i;
	exp->variable_length = 0;//başlangıç değerlerini sıfırlamak. için başka fonksiyon yazarsın

	if (ft_strcmp(str, "$") && ft_strlen(str) == 3)// bu kontrol neden var ?????
		return str;

	sub_str = handle_expansion(shell, str, exp, sub_str);

	if (sub_str)
		return sub_str;
	else
		return str;
}
/* 🔹 g_exitstatus nedir?
g_exitstatus global bir değişken.
Sistemin direkt verdiği bir değişken değil.
waitpid ve execve gibi sistem çağrıları sayesinde çocuğun (yeni process) bitiş kodunu öğreniyorsun.
O öğrendiğin değeri senin shell’in global değişkenine atıyorsun. */


/* char	*handle_g_exitstatus(t_shell *shell, int i, char *str, char *sub_str)
// promt, şu anki index, bir komut, yeni string
{
	char	*nb;
	int		len;

	len = 0;
	nb = 0;
	if (str[i + 1] == '?')//$? → shell’de son çalıştırılan komutun çıkış kodunu gösterir.127 → command not found,
	{
		nb = grbg_itoa(shell, g_exitstatus);
		if (!nb)
			return (NULL);
		len = ft_strlen(nb);
		sub_str = create_sub(shell, str, i, nb, len);
		collect_grbg(shell, sub_str);
	}
	return (sub_str);
}

 */
char	*handle_g_exitstatus(t_shell *shell, int i, char *str, char *sub_str)
{
	char	*exit_code_str;

	exit_code_str = NULL;
	if (str[i + 1] == '?')
	{
		exit_code_str = grbg_itoa(shell, g_exitstatus);
		if (!exit_code_str)
			return (NULL);
		sub_str = create_sub(shell, str, i, exit_code_str, ft_strlen(exit_code_str));
		collect_grbg(shell, sub_str);
	}
	return (sub_str);
}

/*
q[2] // i
q[3] //len
q[4] //
*/
/* char	*handle_expansion(t_shell *shell, char *str, int q[4], char *sub_str)
{
	while (str[q[2]])
	{
		q[0] = (q[0] + (!q[1] && str[q[2]] == '\'')) % 2;
		q[1] = (q[1] + (!q[0] && str[q[2]] == '\"')) % 2;
		if (!q[0] && str[q[2]] == '$' && str[q[2] + 1] && str[q[2] + 1] != ' ')
		{
			if (str[q[2] + 1] == '?')
				sub_str = handle_g_exitstatus(shell, q[2], str, sub_str);
			else
			{
				q[3] = get_len_var(str, q[2] + 1);
				sub_str = create_sub_var(shell, str, q[2], shell->envp, q[3]);
				collect_grbg(shell, sub_str);
				if (sub_str == NULL)
				{
					sub_str = "";
					break ;
				}
			}
			str = sub_str;
		}
		q[2]++;
	}
	return (sub_str);
} */
char *handle_expansion(t_shell *shell, char *str, t_exp *exp, char *sub_str)
//promt, bir komut, exp: indexler, sub_str: yeni string
{
	while (str[exp->current_index])
	{
		// Tırnak kontrolü
		exp->s_quote = (exp->s_quote + (!exp->d_quote && str[exp->current_index] == '\'')) % 2;
		exp->d_quote = (exp->d_quote + (!exp->s_quote && str[exp->current_index] == '\"')) % 2;

		// Eğer tek tırnak içinde değilsek ve $ gördüysek, ve sonrasında boşluk yoksa
		if (!exp->s_quote && str[exp->current_index] == '$' //Tek tırnak '...' içindeki her şey aynen yazılır (expand edilmez), çift tırnak "..." içindekiler ise $VAR gibi değişkenler expand edilir.
			&& str[exp->current_index + 1] && str[exp->current_index + 1] != ' ')
		{
			if (str[exp->current_index + 1] == '?')
				sub_str = handle_g_exitstatus(shell, exp->current_index, str, sub_str);//$? → shell’de son çalıştırılan komutun çıkış kodunu gösterir.127 → command not found,


			else
			{
				exp->variable_length = get_len_var(str, exp->current_index + 1);
				sub_str = create_sub_var(shell, str, exp->current_index, shell->envp, exp->variable_length);
				collect_grbg(shell, sub_str); // Çöp listesine ekle
				if (!sub_str)
				{
					sub_str = "";
					break;
					
				}
			}
			str = sub_str;
		}
		exp->current_index++;
	}
	return sub_str;
}


