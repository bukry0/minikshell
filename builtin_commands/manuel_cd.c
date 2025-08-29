/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manuel_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 01:49:06 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 15:21:44 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_envp(t_shell *shell, char *name)
{
	int		i;
	int		l;
	char	*str;
	char	*env_var;

	i = 0;
	if (!name)
		return (NULL);
	str = grbg_strjoin(shell, name, "=");
	if (!str)
		return (NULL);
	l = ft_strlen(str);
	while (shell->envp[i])
	{
		if (!ft_strncmp(shell->envp[i], str, l))
		{
			env_var = grbg_strdup(shell, shell->envp[i] + l);
			return (env_var);
		}
		i++;
	}
	return (NULL);
}

void	modify_envp(t_shell *shell, char *name, char *insert)
{
	int		i;
	char	*str;

	i = 0;
	if (!shell->envp || !insert || !name)
		return ;
	str = grbg_strjoin(shell, name, "=");
	if (!str)
		return ;
	while (shell->envp[i])
	{
		if (!ft_strncmp(shell->envp[i], str, ft_strlen(str)))
			shell->envp[i] = grbg_strjoin(shell, str, insert);
		i++;
	}
}

static int	go_home_dir(t_shell *shell)
{
	char	*home_dir;
	char	*cwd_before;
	char	*cwd_after;

	home_dir = get_envp(shell, "HOME");
	if (!home_dir)
		return (print_err_msg_lng("cd", "not set", "HOME"));
	cwd_before = (char *)getcwd(NULL, 0);
	collect_grbg(shell, cwd_before);
	modify_envp(shell, "OLDPWD", cwd_before);
	chdir(home_dir);
	cwd_after = (char *)getcwd(NULL, 0);
	collect_grbg(shell, cwd_after);
	modify_envp(shell, "PWD", cwd_after);
	return (0);
}

static int	go_back_dir(t_shell *shell)
{
	char	*old_dir;
	char	*cwd_before;
	char	*cwd_after;

	old_dir = get_envp(shell, "OLDPWD");
	if (!old_dir)
		return (print_err_msg_lng("cd", "not set", "OLDPWD"));
	printf("%s\n", old_dir);
	cwd_before = (char *)getcwd(NULL, 0);
	collect_grbg(shell, cwd_before);
	modify_envp(shell, "OLDPWD", cwd_before);
	chdir(old_dir);
	cwd_after = (char *)getcwd(NULL, 0);
	collect_grbg(shell, cwd_after);
	modify_envp(shell, "PWD", cwd_after);
	return (0);
}

int	manuel_cd(t_cmddat *cmd_data)
{
	DIR		*dir_user;
	char	*cwd_before;
	char	*cwd_after;

	if (cmd_data->parsed_command[1] && cmd_data->parsed_command[2])
		return (print_err_msg_lng("cd", "too many arguments", NULL));
	if (cmd_data->parsed_command)
	{
		if (!cmd_data->parsed_command[1])
			return (go_home_dir(cmd_data->shell));
		else if (!ft_strcmp(cmd_data->parsed_command[1], "-"))
			return (go_back_dir(cmd_data->shell));
	}
	cwd_before = (char *)getcwd(NULL, 0);
	collect_grbg(cmd_data->shell, cwd_before);
	modify_envp(cmd_data->shell, "OLDPWD", cwd_before);
	dir_user = opendir(cmd_data->parsed_command[1]);
	if (!dir_user || chdir(cmd_data->parsed_command[1]) == -1)
		return (print_err_msg_lng("cd", "No such file or directory",
				cmd_data->parsed_command[1]));
	closedir(dir_user);
	cwd_after = (char *)getcwd(NULL, 0);
	collect_grbg(cmd_data->shell, cwd_after);
	modify_envp(cmd_data->shell, "PWD", cwd_after);
	return (0);
}
