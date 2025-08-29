/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 01:24:50 by bcili             #+#    #+#             */
/*   Updated: 2025/08/29 01:29:00 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	signals_interactive(void)
{
	signal(SIGINT, handle_sig_int);
	signal(SIGQUIT, SIG_IGN);
}

void	signals_non_interactive(void)
{
	signal(SIGINT, handle_sig_quit);
	signal(SIGQUIT, handle_sig_quit);
}

void	handle_sig_quit(int n)
{
	if (n == SIGQUIT)
		printf("Quit (core dumped)");
	write(1, "\n", STDERR_FILENO);
	rl_on_new_line();
}

void	handle_sig_int(int n)
{
	if (n == SIGINT)
	{
		g_exitstatus = 130;
		rl_replace_line("", 0);
		rl_on_new_line();
		write(1, "\n", STDERR_FILENO);
		rl_redisplay();
	}
}
