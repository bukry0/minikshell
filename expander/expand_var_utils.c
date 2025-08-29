/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 03:53:37 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 16:03:14 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

ssize_t	get_len_var(char *str, int i)
{
	ssize_t	count;

	count = 0;
	while (str[i] && !ft_isspace(str[i]) && str[i] != '\"' && str[i] != '\''
		&& ft_isalnum(str[i]) && str[i] != '_')
	{
		count++;
		i++;
	}
	return (count);
}

static char	*get_ptr_var(char *str, size_t var_exp_len, char **env)
{
	char	*new;

	while (*env)
	{
		if (!ft_strncmp(str, *env, var_exp_len)
			&& *(*env + var_exp_len) == '=')
		{
			new = *env + var_exp_len + 1;
			if (new == NULL)
				new = ft_strdup("");
			else
				new = ft_strdup(new);
			return (new);
		}
		env++;
	}
	return (NULL);
}

char	*create_sub_var(char *str, size_t i, char **ev, ssize_t len)
{
	char	*s1;
	char	*s2;
	char	*s3;
	char	*expanded_tmp;
	char	*expanded_str;

	expanded_tmp = NULL;
	s1 = ft_substr(str, 0, i);
	s2 = get_ptr_var(&str[i + 1], len, ev);
	s3 = ft_substr(str, i + len + 1, ft_strlen(str) - i - len);
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
}

char	*create_sub(char *str, size_t i, char *nb, ssize_t len)
{
	char	*s1;
	char	*s3;
	char	*expanded_tmp;
	char	*expanded_str;

	s1 = ft_substr(str, 0, i);
	s3 = ft_substr(str, i + len + 1, ft_strlen(str) - i - len);
	expanded_tmp = ft_strjoin(s1, nb);
	expanded_str = ft_strjoin(expanded_tmp, s3);
	free(expanded_tmp);
	free(s1);
	free(s3);
	return (expanded_str);
}
