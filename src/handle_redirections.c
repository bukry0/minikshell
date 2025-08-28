/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 13:37:35 by ecarlier          #+#    #+#             */
/*   Updated: 2025/08/28 11:59:14 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
  Determines the type of redirection for the given string `str`
  and returns the corresponding type value.

  Parameters:
    - str: Pointer to the string representing a command or redirection.

  Returns:
    - 1 if the redirection is an infile-redirection ('<').
    - 2 if the redirection is a here_doc-redirection ('<<').
    - 3 if the redirection is a write to outfile ('>').
    - 4 if the redirection is an append to outfile ('>>').
    - 0 if no redirection type is detected.
*/
/* int	get_type(char *str)
{
	int	type;
	int	i;
	int	q[2];

	i = -1;
	type = 0;
	q[0] = 0;
	q[1] = 0;
	while (str[++i])
	{
		q[0] = (q[0] + (!q[1] && str[i] == '\'')) % 2;
		q[1] = (q[1] + (!q[0] && str[i] == '\"')) % 2;
		if (!q[0] && !q[1])
		{
			if (ft_strlen(str) == 1 && str[i] == '<' )
				type = 1;
			if (str[i] == '<' && str[i + 1] && str[i + 1] == '<')
				type = 2;
			if (type == 0 && str[i] == '>')
				type = 3;
			if (type == 3 && str[i + 1] && str[i + 1] == '>')
				type = 4;
		}
	}
	return (type);
} */
/* <	input redirection	1
<<	heredoc					2
>	output redirection		3
>>	append redirection		4
Diğer	komut/argüman		0 */
int get_type(char *str)
{
    int i = -1;
    int single_quote_open = 0;
    int double_quote_open = 0;

    while (str[++i])
    {
        single_quote_open = (single_quote_open + (!double_quote_open && str[i] == '\'')) % 2;
        double_quote_open = (double_quote_open + (!single_quote_open && str[i] == '\"')) % 2;

        if (!single_quote_open && !double_quote_open)
        {
            if (str[i] == '<' && str[i + 1] && str[i + 1] == '<')//sıralama değiştirildi
                return 2;
            if (str[i] == '<')
                return 1;
            if (str[i] == '>' && str[i + 1] && str[i + 1] == '>')
                return 4;
            if (str[i] == '>')
                return 3;
        }
    }
    return 0;
}


/*
  Handles redirections within the commands
  stored in the shell structure `ptr`.
  It iterates through the command list
  and processes each command to identify and handle redirections.

  Parameters:
    - ptr: Pointer to the shell structure containing the command list.

  Returns: None
*/
void	handle_redir(t_shell *ptr, int type)
{
	int			i;
	t_node		*current_node;//t_node'un datası t_cmdat türünde
	t_cmddat	*cmd_data;

	current_node = ptr->cmd_list;
	while (current_node != NULL)
	{
		cmd_data = current_node->data;
		if (cmd_data)
		{
			i = 0;
			while (cmd_data->parsed_command[i])
			{
				type = get_type(cmd_data->parsed_command[i]);
				if (type < 5 && type > 0)
				{
					open_fd_redir(ptr, cmd_data, i, type);
					cmd_data->parsed_command = del_str(cmd_data->parsed_command, i, 2);
					i -= 1;
				}
				i++;
			}
		}
		current_node = current_node->next;
	}
}

/*
if save_fd > 1 , it means that it's already open and we need to close it
*/

/* int	open_file(char **cmds, int i, int *save_fd, int open_flags, int permission_mode)
{
	if (*save_fd > 1)//önceden açık dosya varsa kapat
	{
		if (close(*save_fd) == -1)
			printf("Error while attempting to close a file");
	}
	if (cmds[i + 1])
	{
		if (io_flags[1] != 0)
			*save_fd = open(cmds[i + 1], io_flags[0], io_flags[1]);
		else
			*save_fd = open(cmds[i + 1], io_flags[0]);
		if (*save_fd == -1)
		{
			g_exitstatus = 1;
			print_err_msg(cmds[i + 1], "No such file or directory");
			return (1);
		}
	}
	else
	{
		syntax_error(NULL, cmds[i + 1]);
	}
	return (0);
} *//* 
int open_file(char **tokens, int redir_index, int *target_fd, int open_flags, int permission_mode)
{
    // Önceden açık bir dosya varsa kapat
    if (*target_fd > 1)
    {
        if (close(*target_fd) == -1)
            printf("Error while attempting to close a file\n");
    }

    // Redirection sembolünden sonra dosya adı var mı?
    if (tokens[redir_index + 1])
    {
        // Dosya oluşturulacaksa → mode parametresi gerekir
        if (io_flags[1] != 0)
            *target_fd = open(tokens[redir_index + 1], io_flags[0], io_flags[1]);
        else
            *target_fd = open(tokens[redir_index + 1], io_flags[0]);

        // Dosya açılamadıysa hata ver
        if (*target_fd == -1)
        {
            g_exitstatus = 1;
            print_err_msg(tokens[redir_index + 1], "No such file or directory");
            return 1;
        }
    }
    else
    {
        // Dosya adı yoksa → sentaks hatası
        syntax_error(NULL, tokens[redir_index + 1]);
    }
    return 0;
} */
int open_file(char **tokens, int redir_index,
        int *target_file, int open_flags, int permission_mode)
{
    char    *filename;

    if (*target_file > 1)
    {
        if (close(*target_file) == -1)
            printf("Error while attempting to close a file\n");
    }
    filename = tokens[redir_index + 1];
    if (!filename)
    {
        syntax_error(NULL, filename);
        return (1);
    }
    if (permission_mode != 0)
        *target_file = open(filename, open_flags, permission_mode);
    else
        *target_file = open(filename, open_flags);
    if (*target_file == -1)
    {
        g_exitstatus = 1;
        print_err_msg(filename, "No such file or directory");
        return (1);
    }
    return (0);
}



/*
Indicates that the file should be...
O_RDONLY: opened in read-only mode.
O_WRONLY: opened in write-only mode.
O_CREAT: created if it does not exist.
O_TRUNC: truncated (emptied) if it already exists.
O_APPEND: Indicates that data should be appended to the
 end of the file during writing.
00644: octal value used to specify the file permissions when creating it.
In this case, 00644 grants read and write permissions to
the file owner, and read permissions to other users.
*/

int	get_flags(int type)
{
		if (type == 1 || type == 2)
			return (O_RDONLY);
		if (type == 3)
			return (O_WRONLY | O_CREAT | O_TRUNC);
		if (type == 4)
			return (O_WRONLY | O_CREAT | O_APPEND);
		return (0);
}
/* #define O_RDONLY    0       // Sadece okuma
#define O_WRONLY    1       // Sadece yazma
#define O_RDWR      2       // Okuma ve yazma
#define O_CREAT     0100    // Dosya yoksa oluştur
#define O_TRUNC     01000   // Dosya varsa içeriğini sil
#define O_APPEND    02000   // Dosya sonuna ekle */

int get_permission_mode(int type)//fonksiyon ikiye bölündü
{
    if (type == 3 || type == 4)
        return 0644;
    return 0; // dosya oluşturulmuyorsa mode gerekmez
}


/* int	open_fd_redir(t_shell *shell, t_cmddat *cmd_struct, int i, int type)
{
	//cmd data = cmd struct
	int	io_flags[2];

	io_flags[0] = get_flags(type, 0);
	io_flags[1] = get_flags(type, 1);
	get_rid_quotes(shell);
	if (type == 1)
		cmd_struct->file_open_flag = open_file(cmd_struct->parsed_command,
				i, &cmd_struct->infile, io_flags);
	else if (type == 2)
		launch_heredoc(shell, cmd_struct, i);
	else if (type == 3)
		cmd_struct->file_open_flag = open_file(cmd_struct->parsed_command,
				i, &cmd_struct->outfile, io_flags);
	else
		cmd_struct->file_open_flag = open_file(cmd_struct->parsed_command,
				i, &cmd_struct->outfile, io_flags);
	return (0);
}
 */

/* 
<	Input redirection	O_RDONLY	0	Komutun stdin'i dosyadan okunur. Dosya zaten var olmalı, oluşturulmaz.
<<	Heredoc	O_RDONLY (geçici dosya okunur)	0	Kullanıcıdan alınan veri geçici dosyaya yazılır, sonra stdin olarak okunur.
>	Output redirection	`O_WRONLY	O_CREAT	O_TRUNC`	0644	Komutun çıktısı dosyaya yazılır. Dosya varsa sıfırlanır, yoksa oluşturulur.
>>	Append redirection	`O_WRONLY	O_CREAT	O_APPEND`	0644	Komutun çıktısı dosyanın sonuna eklenir. Dosya yoksa oluşturulur.
O_RDONLY: Dosya sadece okunur.
O_WRONLY: Dosya sadece yazılır.
O_CREAT: Dosya yoksa oluşturulur.
O_TRUNC: Dosya varsa içeriği sıfırlanır.
O_APPEND: Yazma işlemi dosyanın sonuna eklenir.
0644: UNIX dosya izinleri → rw-r--r--
Eğer open() fonksiyonu O_CREAT bayrağıyla çağrılmıyorsa, mode parametresi yoksayılır.
6	Sahip → read + write	rw-
4	Grup → read only	r--
4	Diğer → read only	r--
Grup	Kimleri kapsar?
Sahip	Dosyayı oluşturan kullanıcı. Genellikle komutu çalıştıran kişi.
Grup	Dosyanın ait olduğu kullanıcı grubu. Aynı projede çalışan kişiler olabilir.
Diğer	Sahip ve grup dışında kalan tüm kullanıcılar. Sistemdeki geri kalan herkes */
 int open_fd_redir(t_shell *shell, t_cmddat *cmd_struct, int i, int type)
{
    int open_flags;
    int permission_mode;

    open_flags = get_flags(type);       // Dosya açma bayrakları
    permission_mode = get_permission_mode(type);  // Dosya izinleri

    get_rid_quotes(shell); // Tırnakları temizle

    if (type == 1) // input redirection: <
        cmd_struct->file_open_flag = open_file(cmd_struct->parsed_command,//file_open_flag' a atılan değeri anlamadım, hata durumunda 1 mi?
            i, &cmd_struct->infile, open_flags, permission_mode);
    else if (type == 2) // heredoc: <<
        launch_heredoc(shell, cmd_struct, i);
    else // output redirection: > veya >>
        cmd_struct->file_open_flag = open_file(cmd_struct->parsed_command,
            i, &cmd_struct->outfile, open_flags, permission_mode);

    return 0;
}
