/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:11:11 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 15:22:59 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern int				g_exitstatus;

typedef struct s_node	t_node;
typedef struct s_shell	t_shell;
typedef struct s_cmddat	t_cmddat;
typedef struct s_grbg	t_grbg;
typedef struct s_exp	t_exp;

typedef struct s_shell
{
	char				*input_string;
	char				**commands_array;
	t_node				*cmd_list;
	char				**envp;
	pid_t				pid;
	int					stop;
	t_grbg				*grbg_lst;
}						t_shell;

typedef struct s_node
{
	t_cmddat			*data;
	struct s_node		*next;
}						t_node;

typedef struct s_cmddat
{
	char				**parsed_command;
	char				*full_path;
	int					infile;
	int					outfile;
	t_shell				*shell;
	int					file_open_flag;
}						t_cmddat;

typedef struct s_grbg
{
	void				*ptr;
	struct s_grbg		*next;
}						t_grbg;

typedef struct s_exp
{
	int	s_quote;
	int	d_quote;
	int	current_index;
	int	variable_length;
}	t_exp;

void		start_main_loop(t_shell *shell);
void		pipe_infile_outfile(t_node *cmd_lst);
void		parser(t_shell *shell);
void		create_cmd_node(t_shell *shell, int start_index,
				int segment_length);
int			count_segment_length(t_shell *shell, int start_index);
void		parse_remaining(t_shell *shell);
char		**fill_arr(t_shell *main_shell, char **shell, int i, int len);
void		check_last_char(t_shell *shell);
void		check_token(t_shell *shell);
void		lexer(t_shell *shell);
char		*handle_spaces(t_shell *shell, char *str, size_t i, int j);
int			special_len(char *str, int count, size_t i);
char		*allocate_mem(t_shell *shell, char *str);
int			check_in_quote(char *str, int index);
int			manuel_cd(t_cmddat *cmd_data);
char		*get_envp(t_shell *shell, char *name);
void		modify_envp(t_shell *shell, char *name, char *insert);
int			manuel_echo(t_cmddat *cmd_data);
int			manuel_env(t_cmddat *cmd_data);
int			is_only_digits(char *s);
int			manuel_exit(t_cmddat *cmd_data);
int			manuel_export(t_cmddat *cmd);
int			print_export(t_cmddat *cmd);
void		print_line_export(t_cmddat *cmd, int i);
int			get_len_id(t_shell *shell, char *str, int msg);
int			scan_envp(t_cmddat *cmd, char *str, int id_len);
int			manuel_pwd(t_cmddat *cmd_data);
int			manuel_unset(t_cmddat *cmd);
size_t		get_len_env(char *s);
char		*indicate_path_cmds(t_cmddat *cmd, char **ev);
char		*get_path(char *cmd, char **ev, size_t i);
void		free_split(char **strs);
void		add_last_cmd_to_envp(t_shell *shell);
void		wait_update_exitstatus(t_shell *shell);
void		cls_fds(void *content);
int			is_workable_path(t_cmddat *cmd_data, int builtin);
int			is_executable(t_cmddat *cmd_data);
void		run_cmd(void *content);
int			execute_cmds(t_shell *shell);
void		exit_ms(int g_exitstatus, t_shell *shell);
void		free_all(t_shell *shell);
void		free_node_list(t_node *head);
ssize_t		get_len_var(char *str, int i);
char		*create_sub_var(char *str, size_t i, char **ev, ssize_t len);
char		*create_sub(char *original_str, size_t var_index, char *replacement,
				ssize_t var_len);
char		*create_expanded_str(char *original_str, size_t var_index,
				char *var_value, ssize_t var_len);
char		*find_env_value(t_shell *shell, char *str, t_exp *exp,
				char *sub_str);
char		*handle_expansion(t_shell *shell, char *str, t_exp *exp,
				char *sub_str);
char		*handle_g_exitstatus(t_shell *shell, int i, char *str,
				char *sub_str);
char		*expand_var(t_shell *shell, char *str, int i);
char		**expander(t_shell *shell);
void		*get_grbg(t_shell *shell, size_t nmemb, size_t size);
void		collect_grbg(t_shell *shell, void *new);
void		free_grbg(t_grbg *head);
char		*grbg_strjoin(t_shell *shell, char *s1, char *s2);
char		*grbg_strdup(t_shell *shell, char *s);
char		*grbg_substr(t_shell *shell, char *s, unsigned int start,
				size_t len);
char		*grbg_itoa(t_shell *shell, int n);
int			get_type(char *str);
void		handle_redir(t_shell *ptr, int type);
int			open_file(t_cmddat *cmd_struct, int redir_index,
				int open_flags, int permission_mode);
int			get_flags(int type);
int			get_permission_mode(int type);
void		launch_heredoc(t_shell *shell, t_cmddat *cmd, int i);
int			open_fd_redir(t_shell *shell, t_cmddat *cmd_struct,
				int i, int type);
int			get_heredoc(t_shell *shell, char *lim);
int			pipe_heredoc(t_shell *shell, char *content);
void		cstm_lstiter(t_node *lst, void (*f)(void *));
t_node		*cstm_lstlast(t_node *lst);
int			cstm_lstsize(t_node*lst);
void		cstm_lstclear(t_node **lst, void (*del)(void *));
void		cstm_lstdelone(t_node *lst, void (*del)(void *));
void		add_node_to_list(t_shell *shell, t_node **head, t_cmddat *data);
int			ft_listsize(t_node *lst);
void		trim_quotes(t_shell *shell);
char		*prepare_trimmed_buffer(t_shell *shell, char *input);
char		*get_trimmed(t_shell *shell, char *input);
int			malloc_len(char *str);
char		**ft_create_substrs(t_shell *shell, char **substrings,
				char *input, char *separators);
char		**split_input(char *str, t_shell *shell);
char		*add_to_str(t_shell *shell, char **str, char *add);
size_t		get_len_arr(char **array);
char		**del_str(char **array, int pos, int count);
char		**add_str_to_arr(t_shell *shell, char **arr, char *str);
int			syntax_error(t_shell *shell, char *token);
void		print_err_msg(char *cmd, char *msg);
int			print_err_msg_lng(char *cmd, char *msg, char *arg);
int			check_double_pipes(t_shell *shell);
int			ft_isspace(int c);
int			get_builtin_nbr(t_cmddat *cmd);
int			execute_builtin(t_cmddat *cmd, int n, int forked);
void		signals_interactive(void);
void		signals_non_interactive(void);
void		handle_sig_quit(int n);
void		handle_sig_int(int n);
void		init_shell_struct(t_shell *shell, char **envp);
int			init_env(t_shell *shell, char **env);
t_cmddat	*init_struct_cmd(t_shell *shell);

#endif