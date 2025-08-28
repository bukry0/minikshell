/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 13:57:41 by ecarlier          #+#    #+#             */
/*   Updated: 2025/08/28 11:54:13 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
Iterate over the two-dimensional array
Whenever a redirection is found, check the type of
redirection and retrieve a file descriptor containing t
he info we need as the infile
Check that the file descriptor that has been opened is valid (!= -1)
and continue
If a pipe is found, add a new node to the list of commands
In all other cases add whatever words are found
 to the argument list (argv) we call parsed_command
*/

/*
  Creates and fills a new array of strings from the `shell` array.
  It starts filling from index `i` and fills `len` number of elements.
  Memory is dynamically allocated for the new array.

  Parameters:
	  main_shell: Pointer to the shell structure
	- shell: Pointer to the original array of strings.
	- i:      Starting index to begin filling from.
	- len:    Number of elements to fill in the new array.

  Returns:
	- Pointer to the newly created array of strings.
*/
char **fill_arr(t_shell *shell, char **promt_commands, int start_index, int seg_len)
{
	char **temp;
	int i;

	i = 0;
	temp = NULL;
	temp = get_grbg(shell, seg_len + 1, sizeof(char *));
	if (!temp)
		return (NULL);
	while (seg_len > 0)
	{
		temp[i] = grbg_strdup(shell, promt_commands[start_index]);
		start_index++;
		i++;
		seg_len--;
	}
	temp[i] = NULL;
	return (temp);
}

/*
Checks if the last character of
the command in the shell is either '<', '>', or '|'.
If it's the case, shell->stop = 1 and throw syntax error.
Parameters:
	- shell: Pointer to the shell structure containing input commands.

Returns:
- No return value.
*/
static void check_last_char(t_shell *shell)
{
	int len_ar;
	char last_char;

	if (check_double_pipes(shell) == 1)
	{
		shell->stop = 1;
		return;
	}
	len_ar = get_len_arr(shell->commands_array) - 1;
	if (len_ar < 0 || shell->commands_array[len_ar] == NULL // son komutun null olması neden geçersiz
		|| shell->commands_array[len_ar][0] == '\0')
		return;
	last_char = shell->commands_array[len_ar][0];
	if (last_char == '|' || last_char == '<' || last_char == '>')
	{
		shell->stop = 1;
		syntax_error(shell, shell->commands_array[len_ar]);
	}
}

/*
Checks if the last character of EACH command in the shell is either '<', '>'.
If it's the case, shell->stop = 1 and throw syntax error
Parameters:
	- shell: Pointer to the shell structure containing input commands.

Returns:
- No return value.
*/
static void check_token(t_shell *shell)
{
	t_node *current_node;
	t_cmddat *cmd_data;
	int len_arr;
	char last_char;

	current_node = shell->cmd_list;//Komut listesinin başından başlıyoruz Her t_node, bir komutu (t_cmddat) içeriyor.
	while (current_node != NULL)
	{
		cmd_data = current_node->data;
		len_arr = get_len_arr(cmd_data->parsed_command) - 1;//Son elemanın indeksini buluyoruz.
		last_char = cmd_data->parsed_command[len_arr][0];//son tokenın ilk karkterini alıyoruz
		if (last_char == '|' || last_char == '<' || last_char == '>')//neden tekrar kontrol etme gereği duymuş
		{
			shell->stop = 1;
			syntax_error(shell, cmd_data->parsed_command[len_arr]);
			free_node_list(shell->cmd_list);
			shell->cmd_list = NULL;
			break;
		}
		current_node = current_node->next;
	}
}
/* shell->cmd_list → [t_node]
                    ↳ data → [t_cmddat]
                            ↳ parsed_command = ["echo", "merhaba"]
                            ↳ infile = NULL
                            ↳ outfile = NULL
                            ↳ full_path = "/bin/echo"
                    ↳ next → [t_node]
                                ↳ data → [t_cmddat]
                                        ↳ parsed_command = ["grep", "a"]
                                        ↳ ... */

/*might lead to double frees with gc,
but seems to work @DENIZ DOUBLE FREE if error
@deniz is this comment still accurate (22/04)*/
static void parse_remaining(t_shell *shell)
{
	int type;

	type = 0;
	check_token(shell);
	if (shell->stop == 0)
		handle_redir(shell, type);
	if (shell->cmd_list != NULL)
		add_last_cmd_to_envp(shell);
	get_rid_quotes(shell);
}

/* void parser(t_shell *shell, int cmd_index, int segment_length)
{
	//ORNEK: echo merhaba | grep a | wc -l
	// i, j = 0
	t_cmddat *ptr;

	ptr = NULL;
	check_last_char(shell);
	while (shell && shell->commands_array && shell->commands_array[i] != NULL && shell->stop == 0)
	{
		ptr = init_struct_cmd(shell);
		if (!ptr)
			return;
		add_node_to_list(shell, &(shell->cmd_list), ptr);
		while (shell->commands_array[cmd_index] != NULL && shell->commands_array[cmd_index][0] != '|')//pipe a kadar devam et
		{
			cmd_index++;
			segment_length++;
		}
		ptr->parsed_command = fill_arr(shell, shell->commands_array, cmd_index - segment_length, segment_length);
		/* 		cmd_list adlı bağlı liste oluşur.
				Her node bir komutu (t_cmddat) tutar.
				Executor bu listeyi sırayla dolaşarak komutları çalıştırır. 
		ptr->full_path = get_path_cmds(ptr, shell->envp);
		collect_grbg(shell, ptr->full_path);
		if (shell->commands_array[cmd_index] == NULL)
			break;
		cmd_index++;
		segment_length = 0;
	}
	parse_remaining(shell);
} */

//FONSKİYON PROTOTİPLERİNİ HEADER'A TANIMLAMAYI UNUTMA!!

int count_segment_length(t_shell *shell, int start_index)
{
    int length = 0;

    while (shell->commands_array[start_index] &&
           shell->commands_array[start_index][0] != '|')
    {
        start_index++;
        length++;
    }
    return length;
}
void create_cmd_node(t_shell *shell, int start_index, int segment_length)
{
    t_cmddat *cmd_data;

    cmd_data = init_struct_cmd(shell);
    if (!cmd_data)
        return;

    cmd_data->parsed_command = fill_arr(shell, shell->commands_array, start_index, segment_length);
    cmd_data->full_path = get_path_cmds(cmd_data, shell->envp);
    collect_grbg(shell, cmd_data->full_path);
    add_node_to_list(shell, &(shell->cmd_list), cmd_data);
}

void parser(t_shell *shell)
{
    int index = 0;

    check_last_char(shell);

    while (shell && shell->commands_array && shell->commands_array[index] && !shell->stop)
    {
        int segment_length = count_segment_length(shell, index);

        create_cmd_node(shell, index, segment_length);

        // index'i pipe'tan sonraki komuta geçir
        index += segment_length;
        if (shell->commands_array[index] == NULL)
            break;
        index++; // '|' karakterini atla
    }

    parse_remaining(shell);
}


