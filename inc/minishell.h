/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:26:53 by ecarlier          #+#    #+#             */
/*   Updated: 2025/08/28 12:31:26 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern int				g_exitstatus;

// grbg_substr içine shell de göndermişiz ama bazı kullanıldığı fonksiyonlarda shell yok

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
	int s_quote;         // Tek tırnak içinde mi (0 = hayır, 1 = evet)
	int d_quote;         // Çift tırnak içinde mi (0 = hayır, 1 = evet)
	int current_index;   // Str üzerinde gezinilen karakterin indeksi
	int variable_length; // Bulunan değişkenin uzunluğu
} t_exp;

/* exit_minishell*/
void					free_node_list(t_node *head);
void					free_all(t_shell *shell);
void					exit_ms(int g_exitstatus, t_shell *shell);

/*	main	*/
void					launch_minishell(t_shell *shell);
void					pipe_infile_outfile(t_node *cmd_lst);

/*	init	*/
void					init_shell_struct(t_shell *shell, char **envp);
int						init_env(t_shell *shell, char **env);
t_cmddat				*init_struct_cmd(t_shell *shell);

/*	lexer */
void					lexer(t_shell *shell);

char					**split_input(char *str, t_shell *shell);
char					**ft_split(char const *s, char c);
char					*handle_spaces(t_shell *shell, char *str, size_t i,
							int j);
char					**ft_create_substrs(t_shell *p, char **aux,
							char const *s, char *set);

/* quotes_utils */
void					get_rid_quotes(t_shell *shell);
char					*get_trimmed(t_shell *shell, char const *s1,
							int squote, int dquote);
int						malloc_len(char const *str);

/*	parser*/
void					parser(t_shell *shell);//degistirdim
char					**fill_arr(t_shell *main_shell, char **shell, int i,
							int len);
void					add_last_cmd_to_envp(t_shell *p);
int						check_double_pipes(t_shell *shell);

/* handle_redirections */
int						get_type(char *str);
void					handle_redir(t_shell *ptr, int type);
int						open_file(char **cmds, int i, int *save_fd,
							int io_flags[2]);
int						get_flags(int type, int file_access_type);
int						open_fd_redir(t_shell *shell, t_cmddat *cmd_struct,
							int i, int type);
int						syntax_error(t_shell *shell, char *token);

/* here_doc */
void					launch_heredoc(t_shell *shell, t_cmddat *cmd, int i);
int						get_heredoc(t_shell *shell, char *lim);
int						pipe_heredoc(char *content);

/*	builtins	*/
int						get_builtin_nbr(t_cmddat *cmd);
int						execute_builtin(t_cmddat *cmd, int n, int forked);

/*	cstm_echo	*/
int						cstm_echo(t_cmddat *cmd_data);

/*	cstm_pwd	*/
int						cstm_pwd(t_cmddat *cmd_data);

/*	cstm_env	*/
int						cstm_env(t_cmddat *cmd_data);

/*	cstm_exit	*/
int						cstm_exit(t_cmddat *cmd_data);
int						is_only_digits(char *s);

/*	cstm_unset	*/
int						cstm_unset(t_cmddat *cmd);
size_t					get_len_env(const char *s);

/*	cstm_cd	*/
int						cstm_cd(t_cmddat *cmd_data);
int						go_home_dir(t_shell *shell);
int						go_back_dir(t_shell *shell);
void					modify_envp(t_shell *shell, char *name, char *insert);
char					*get_envp(t_shell *shell, char *name);

/*	cstm_export	*/
int						cstm_export(t_cmddat *cmd);
int						print_export(t_cmddat *cmd);
void					print_line_export(t_cmddat *cmd, int i);
int						get_len_id(t_shell *shell, char *str, int msg);
int						scan_envp(t_cmddat *cmd, char *str, int id_len);

/*	exec	*/
int						execute_cmds(t_shell *shell);
void					cls_fds(void *content);
void					run_cmd(void *content);
void					wait_update_g_exitstatus(t_shell *shell);
int						is_executable(t_cmddat *cmd_data);

/*	signal_handler	*/
void					sigint_handler(int signum);
void					handle_sig_quit(int n);
void					signals_interactive(void);
void					handle_sig_int(int n);
void					signals_non_interactive(void);

/*	garbage_collector	*/
void					*get_grbg(t_shell *shell, size_t nmemb, size_t size);
void					collect_grbg(t_shell *shell, void *new);
void					free_grbg(t_grbg *head);

/*	garbage_lib	*/
char					*grbg_strdup(t_shell *shell, const char *s);
char					*grbg_substr(t_shell *shell, char const *s,
							unsigned int start, size_t len);
char					*grbg_itoa(t_shell *shell, int n);
char					*grbg_strjoin(t_shell *shell, char const *s1,
							char const *s2);

/*	list_functions */
void					cstm_lstiter(t_node *lst, void (*f)(void *));
t_node					*cstm_lstlast(t_node *lst);
int						cstm_lstsize(t_node *lst);
void					cstm_lstclear(t_node **lst, void (*del)(void *));
void					cstm_lstdelone(t_node *lst, void (*del)(void *));

/*	utils	*/
size_t					get_len_arr(char **array);
void					print_err_msg(char *cmd, char *msg);
int						print_err_msg_lng(char *cmd, char *msg, char *arg);
int						ft_isspace(int c);
char					**del_str(char **array, int pos, int count);
char					**add_str_to_arr(t_shell *shell, char **arr,
							char *str);
char					*add_to_str(t_shell *shell, char **str, char *add);

/* list_utils */
void					add_node_to_list(t_shell *shell, t_node **head,
							t_cmddat *data);
int						ft_listsize(t_node *lst);

/* envp_utils*/
char					*get_path_cmds(t_cmddat *cmd, char **ev);
char					*get_path(char *cmd, char **ev, size_t i);
void					free_split(char **strs);

/* expander */
char					**expander(t_shell *shell);
char					*handle_g_exitstatus(t_shell *shell, int i, char *str,
							char *sub_str);

/*expand_var_utils */
char					*create_sub_var(t_shell *shell, char *str, size_t i, char **ev,
							ssize_t len);
ssize_t					get_len_var(char *str, int i);
char					*create_sub(t_shell *shell, char *str, size_t i, char *nb, ssize_t len);
char					*expand_var(t_shell *shell, char *str, char **ev,
							int i);
char					*handle_expansion(t_shell *shell, char *str, t_exp *exp,
							char *sub_str);

/* utils2 */
/* int	ft_isspace(char c)
 */
#endif
