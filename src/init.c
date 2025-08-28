/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 20:12:21 by ecarlier          #+#    #+#             */
/*   Updated: 2025/08/28 11:59:30 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*----------------------------------------------------------------------------------*/
/*
Initializes the shell structure `shell` with the
provided environment variables `envp`.

Parameters:
- shell: Pointer to the shell structure to initialize.
- envp:   Array of strings representing the environment variables.

Note: This function also initializes the environment
within the shell structure using the `init_env` function.

Returns: None
*/
/*----------------------------------------------------------------------------------*/
/* Özet Liste
get_grbg ile bellek ayrılır → collect_grbg ile çöp listesine yeni node eklenir.
shell->envp ve her ortam değişkeni stringi için bu işlemler tekrar edilir.
Çöp listesi, tüm bu adresleri node'lar halinde saklar.
Program sonunda çöp listesi dolaşılarak tüm bellekler serbest bırakılır. */
/*----------------------------------------------------------------------------------*/

void	init_shell_struct(t_shell *shell, char **envp)
{
	shell->input_string = NULL;
	shell->commands_array = NULL;
	shell->cmd_list = NULL;
	shell->pid = -1;//şu an geçerli pid yok
	shell->stop = 0;
	shell->grbg_lst = NULL;
	init_env(shell, envp);
}

/*
Initializes the environment for the minishell by copying the provided
environment variables `env` into the `shell->envp` array.

Parameters:
- shell: Pointer to the shell structure containing environment information.
- env:    Array of strings representing the environment variables.

Returns:
  - 1 if the initialization is successful.
  - 0 if an error occurs during memory allocation or copying.
*/
int	init_env(t_shell *shell, char **env)
{
	int	i;

	i = 0;
	shell->envp = get_grbg(shell, get_len_arr(env) + 1, sizeof(char *)); //get_len_arr ile kaç tane ortam değişken olduğunu bulup o kadar yer ayırıyoruz.
/*  hâlâ bir double pointer var.
 Sadece, bellekte bir dizi pointer için yer ayırıp,
 onun adresini char ** olarak kullanıyorsun. */
	if (!shell->envp)
	{
		printf("Error allocating memory for envp\n");
		return (0);
	}
	while (env[i])
	{
		shell->envp[i] = grbg_strdup(shell, env[i]);
		if (!shell->envp[i])
		{
			printf("Error env");
			return (0);
			// g_exitstatus = 1; olması gerekmiyor mu ????
		}
		i++;
	}
	shell->envp[i] = NULL;
	return (1);
}

/*
  Initializes a new structure t_cmddat with default values.
  Allocates memory for the structure and returns a pointer to it.

  Returns:
    - Pointer to the newly initialized t_cmddat structure.
*/
t_cmddat	*init_struct_cmd(t_shell *shell)
{
	t_cmddat	*ptr;

	ptr = get_grbg(shell, 1, sizeof(t_cmddat));
	if (!ptr)
		return (NULL);
	ptr->parsed_command = NULL;
	ptr->full_path = NULL;
	ptr->infile = STDIN_FILENO;
	ptr->outfile = STDOUT_FILENO;
	ptr->shell = shell;
	ptr->file_open_flag = 0;
	return (ptr);
}
