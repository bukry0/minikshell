/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 07:17:31 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 15:12:03 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**remove_quotes_from_tokens(t_shell *shell, char **tokens)
{
	int		i;
	int		len;
	char	**new_tokens;

	i = 0;
	len = get_len_arr(tokens);
	new_tokens = get_grbg(shell, len + 1, sizeof(char *));
	if (!new_tokens)
		return (NULL);
	while (tokens[i])
	{
		new_tokens[i] = get_trimmed(shell, tokens[i]);
		i++;
	}
	new_tokens[i] = NULL;
	return (new_tokens);
}

void	trim_quotes(t_shell *shell)
{
	t_node		*current_node;
	t_cmddat	*cmd_data;

	current_node = shell->cmd_list;
	while (current_node)
	{
		cmd_data = current_node->data;
		cmd_data->parsed_command = remove_quotes_from_tokens(shell,
				cmd_data->parsed_command);
		current_node = current_node->next;
	}
}

char	*prepare_trimmed_buffer(t_shell *shell, char *input)
{
	int		quote_count;
	int		buffer_size;
	char	*buffer;

	if (!input)
		return (NULL);

	quote_count = malloc_len(input);
	if (quote_count == -1)
		return (NULL);
	buffer_size = ft_strlen(input) - quote_count + 1;
	if (buffer_size <= 0)
		return (NULL);
	buffer = get_grbg(shell, buffer_size, sizeof(char));
	return (buffer);
}

char	*get_trimmed(t_shell *shell, char *input)
{
	int		source_index;
	int		target_index;
	char	*trimmed;

	target_index = -1;
	source_index = 0;
	trimmed = prepare_trimmed_buffer(shell, input);
	if (!trimmed)
		return (NULL);

	while (input[source_index])
	{

		if (!check_in_quote(input, source_index))
		{
			trimmed[++target_index] = input[source_index];
		}
		source_index++;
	}
	trimmed[++target_index] = '\0';
	return (trimmed);
}

int	malloc_len(char *str)
{
	int	count;
	int	squote;
	int	dquote;
	int	i;

	count = 0;
	squote = 0;
	dquote = 0;
	i = 0;
	while (str[i])
	{
		if (check_in_quote(str, i))
			count++;
		i++;
	}
	if (squote || dquote)
		return (-1);
	return (count);
}
