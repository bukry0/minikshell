/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 21:43:45 by ecarlier          #+#    #+#             */
/*   Updated: 2025/08/28 12:26:21 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
Gets the lenght of the var to expand, starting from after the$
i is the position of the string */
ssize_t	get_len_var(char *str, int i)
{
	ssize_t	count;

	count = 0;
	while (str[i] && !ft_isspace(str[i]) && str[i] != '\"' && str[i] != '\'' )
	{
		count++;
		i++;
	}
	return (count);
}

/*
Searches for the value of the environment variable
specified by the string `str` within the array of environment variables `env`.
It returns a pointer to the value of the environment variable.

Parameters:
- str:          Pointer to the string representing the environment variable.
- var_exp_len:  Length of the environment variable to search for.
- env:          Array of strings representing environment variables.

Returns:
- Pointer to the value of the environment variable if found.
- Pointer to an empty string if the environment variable is not found.
*/
static char	*get_ptr_var(char *str, size_t var_exp_len, char **env)
{
	while (*env)
	{
		if (!ft_strncmp(str, *env, var_exp_len)
			&& *(*env + var_exp_len) == '=')
			return (*env + var_exp_len + 1);
		env++;
	}
	return (NULL);
}

/*
Creates a substring by expanding the environment
variable specified by the substring in the string
`str`. It constructs and returns a new string with the
 expanded environment variable.

Parameters:
- str: Pointer to the string containing the environment
variable to be expanded.
- i:   Index of the starting character of the environment variable substring.
- ev:  Array of strings representing environment variables.
- len: Length of the environment variable substring to expand.

  Returns:
    - Pointer to the newly created string with the expanded environment variable.
*/
/* char	*create_sub_var(t_shell *shell, char *str, size_t i, char **ev, ssize_t len)
{
	char	*s1;
	char	*s2;
	char	*s3;
	char	*expanded_tmp;
	char	*expanded_str;

	expanded_tmp = NULL;
	s1 = grbg_substr(shell, str, 0, i);
	s3 = grbg_substr(shell, str, i + len + 1, ft_strlen(str) - i - len);
	s2 = ft_strdup(get_ptr_var(&str[i + 1], len, ev));
	if (s2 != NULL )
	{
		expanded_tmp = ft_strjoin(s1, s2);
		expanded_str = ft_strjoin(expanded_tmp, s3);
	}
	free(s1);
	free(s3);
	if (s2 == NULL)
		return (NULL);
	else
	{
		free(s2);
		free(expanded_tmp);
	}
	return (expanded_str);
} */
/* char *create_sub_var(t_shell *shell, char *str, size_t i, char **ev, ssize_t len)
{
	char *prefix;//komut komut gönderildiği için $VAR'dan öncesi olabilir mi? öncesini tutmaya gerek var mı yoksa sadece ilk indexe $ koymak yeterli mi?
	char *var_value;
	char *suffix;
	char *temp_str;
	char *result_str;

	temp_str = NULL;
	prefix = grbg_substr(shell, str, 0, i);
	suffix = grbg_substr(shell, str, i + len + 1, ft_strlen(str) - i - len);
	var_value = ft_strdup(get_ptr_var(&str[i + 1], len, ev));

	if (var_value != NULL)
	{
		temp_str = ft_strjoin(prefix, var_value);
		result_str = ft_strjoin(temp_str, suffix);
	}

	free(prefix);
	free(suffix);

	if (var_value == NULL)
		return NULL;
	else
	{
		free(var_value);
		free(temp_str);
	}

	return result_str;
} */
//EGER PREFIX SADECE $ ISE BU KOD GECERLI
char *create_sub_var(t_shell *shell, char *str, size_t i, char **ev, ssize_t len)
{
	char *var_value;
	char *suffix;
	char *temp_str;
	char *result_str;

	temp_str = NULL;
	// suffix: $VAR’dan sonraki kısım
	suffix = grbg_substr(shell, str, i + len + 1, ft_strlen(str) - i - len);

	// değişkenin değerini bul
	var_value = ft_strdup(get_ptr_var(&str[i + 1], len, ev));

	if (var_value != NULL)
	{
		// $ işareti başa konur
		temp_str = ft_strjoin("$", var_value);
		// suffix eklenir
		result_str = ft_strjoin(temp_str, suffix);
	}

	free(suffix);

	if (var_value == NULL)
		return NULL;
	else
	{
		free(var_value);
		free(temp_str);
	}

	return result_str;
}



/* char	*create_sub(t_shell *shell, char *str, size_t i, char *nb, ssize_t len)//create_sub_var ile birleştirilebilir mi
{
	//str, i, exit_code_str, ft_strlen(exit_code_str)
	char	*s1;
	char	*s3;
	char	*expanded_tmp;
	char	*expanded_str;

	s1 = grbg_substr(shell, str, 0, i);
	s3 = grbg_substr(shell, str, i + len + 1, ft_strlen(str) - i - len);
	expanded_tmp = ft_strjoin(s1, nb);
	expanded_str = ft_strjoin(expanded_tmp, s3);
	free(expanded_tmp);
	free(s1);
	free(s3);
	return (expanded_str);
} */

char	*create_sub(t_shell *shell, char *original_str, size_t var_index, char *replacement,
					ssize_t var_len)
{
		//komut, i, exit_code_str, ft_strlen(exit_code_str)

	char	*prefix;
	char	*suffix;
	char	*temp_str;
	char	*result_str;

	prefix = grbg_substr(shell, original_str, 0, var_index);
	suffix = grbg_substr(shell, original_str, var_index + var_len + 1,
			ft_strlen(original_str) - var_index - var_len);
	temp_str = ft_strjoin(prefix, replacement);
	result_str = ft_strjoin(temp_str, suffix);
	free(temp_str);
	free(prefix);
	free(suffix);
	return (result_str);
}
//CREATE_SUB_VAR & CREATE_SUB BIRLESTIRILMIS HALI -- BUKET BURAYI KONTROLEDER MISIN <3 
char	*create_expanded_str(t_shell *shell, char *original_str, size_t var_index,
				char *var_value, ssize_t var_len)
{
	char	*prefix;
	char	*suffix;
	char	*temp_str;
	char	*result_str;

	if (!var_value) // değişken değeri yoksa NULL döndür
		return (NULL);

	prefix = grbg_substr(shell, original_str, 0, var_index);
	suffix = grbg_substr(shell, original_str, var_index + var_len + 1,
			ft_strlen(original_str) - var_index - var_len);
	temp_str = ft_strjoin(prefix, var_value);
	result_str = ft_strjoin(temp_str, suffix);

	free(temp_str);
	free(prefix);
	free(suffix);
	return (result_str);
}
