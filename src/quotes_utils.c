/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 11:39:46 by ecarlier          #+#    #+#             */
/*   Updated: 2025/08/28 11:54:23 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
  Removes unnecessary quotes from the commands stored in the shell.
  It iterates over each command, trims unnecessary quotes from its arguments,
  and updates the shell's command data accordingly.

  Parameters:
    - shell: Pointer to the shell containing the commands to be processed.

  Returns:
    - None.
*/
/* void	get_rid_quotes(t_shell	*shell)
{
	char		**temp;
	char		*trim_cmd;
	int			i[2];
	t_node		*current_node;
	t_cmddat	*cmd_data;

	current_node = shell->cmd_list;
	while (current_node != NULL)
	{
		cmd_data = current_node->data;
		i[0] = 0;
		i[1] = get_len_arr(cmd_data->parsed_command);
		temp = get_grbg(shell, i[1] + 1, sizeof(char *));
		if (!temp)
			return ;
		while (cmd_data->parsed_command[i[0]])
		{
			trim_cmd = get_trimmed(shell, cmd_data->parsed_command[i[0]], 0, 0);
			temp[i[0]] = trim_cmd;
			i[0]++;
		}
		cmd_data->parsed_command = temp;
		current_node = current_node->next;
	}
} */
static char **remove_quotes_from_tokens(t_shell *shell, char **tokens)
{
    int i = 0;
    int len = get_(tokens);
    char **new_tokens = get_grbg(shell, len + 1, sizeof(char *));
    if (!new_tokens)
        return NULL;

    while (tokens[i])
    {
        new_tokens[i] = get_trimmed(shell, tokens[i], 0, 0);
        i++;
    }
    new_tokens[i] = NULL;
    return new_tokens;
}
void get_rid_quotes(t_shell *shell)
{
    t_node *current_node = shell->cmd_list;
    t_cmddat *cmd_data;

    while (current_node)
    {
        cmd_data = current_node->data;
        cmd_data->parsed_command = remove_quotes_from_tokens(shell, cmd_data->parsed_command);
        current_node = current_node->next;
    }
}
void get_rid_quotes(t_shell *shell)
{
    char **temp;
    char *trimmed_token;
    int token_index;
    int token_count;
    t_node *current_node;
    t_cmddat *cmd_data;

    current_node = shell->cmd_list;
    while (current_node != NULL)
    {
        cmd_data = current_node->data;
        token_index = 0;
        token_count = get_len_arr(cmd_data->parsed_command);

        temp = get_grbg(shell, token_count + 1, sizeof(char *));
        if (!temp)
            return ;

        while (cmd_data->parsed_command[token_index])
        {
            trimmed_token = get_trimmed(shell, cmd_data->parsed_command[token_index], 0, 0);
            temp[token_index] = trimmed_token;
            token_index++;
        }
        cmd_data->parsed_command = temp;
        current_node = current_node->next;
    }
}


/*
  Removes unnecessary quotes from a str while preserving quoted substrings.
  It returns a new dynamically allocated string containing the trimmed result.

  Parameters:
    - s1: Pointer to the input string to be trimmed.
    - squote: Integer representing the state of single quotes in the string.
    - dquote: Integer representing the state of double quotes in the string.

  Returns:
    - Pointer to the dynamically allocated trimmed string.
*/
/* char	*get_trimmed(t_shell *shell, char const *s1, int squote, int dquote)
{
	int		count;
	int		i[2];
	char	*trimmed;

	i[1] = -1;
	i[0] = 0;
	count = malloc_len(s1);
	if (!s1 || count == -1)
		return (NULL);
	trimmed = get_grbg(shell, ft_strlen(s1) - count + 1, sizeof(char));
	if (!trimmed)
		return (NULL);
	while (s1[i[0]])
	{
		squote = (squote + (!dquote && s1[i[0]] == '\'')) % 2;
		dquote = (dquote + (!squote && s1[i[0]] == '\"')) % 2;
		if ((s1[i[0]] != '\"' || squote) && (s1[i[0]] != '\'' || dquote) \
			&& ++i[1] >= 0)
			trimmed[i[1]] = s1[i[0]];
		i[0]++;
	}
	trimmed[++i[1]] = '\0';
	return (trimmed);
} */
char *prepare_trimmed_buffer(t_shell *shell, const char *input)//ekleme yapıldı
{
    int quote_count;
    int buffer_size;
    char *buffer;

    if (!input)
        return NULL;

    quote_count = malloc_len(input);
    if (quote_count == -1)
        return NULL;

    buffer_size = ft_strlen(input) - quote_count + 1;
    if (buffer_size <= 0)
        return NULL;

    buffer = get_grbg(shell, buffer_size, sizeof(char));
    return buffer;
}

char *get_trimmed(t_shell *shell, char const *input, int single_quote_open, int double_quote_open)
{
    int source_index = 0;
    int target_index = -1;
    int quote_count;
    char *trimmed;
    
	trimmed = prepare_trimmed_buffer(shell, input);
	
	if (!trimmed)
    	return NULL;

    while (input[source_index])
    {
        single_quote_open = (single_quote_open + (!double_quote_open && input[source_index] == '\'')) % 2;
        double_quote_open = (double_quote_open + (!single_quote_open && input[source_index] == '\"')) % 2;

        if ((input[source_index] != '\"' || single_quote_open) &&
            (input[source_index] != '\'' || double_quote_open))
        {
            trimmed[++target_index] = input[source_index];
        }
        source_index++;
    }
    trimmed[++target_index] = '\0';
    return trimmed;
}


/*
  Counts the number of unnecessary quotes in the string `str`.
  It checks the consistency of quotes and returns the count.

  Parameters:
    - str: Pointer to the input string to be checked.

  Returns:
    - Number of unnecessary quotes in the string.
*/
int	malloc_len(char const *str)
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
		squote = (squote + (!dquote && str[i] == '\'')) % 2;
		dquote = (dquote + (!squote && str[i] == '\"')) % 2;
		if ((str[i] == '\"' && !squote) || (str[i] == '\'' && !dquote))
			count++;
		i++;
	}
	if (squote || dquote)
		return (-1);
	return (count);
}
