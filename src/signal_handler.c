/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcili <bcili@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:55:10 by ecarlier          #+#    #+#             */
/*   Updated: 2025/08/28 11:49:08 by bcili            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	signals_interactive(void)
{
	signal(SIGINT, handle_sig_int);
	signal(SIGQUIT, SIG_IGN);
}

void	signals_non_interactive(void)
{
	signal(SIGINT, handle_sig_quit); //(Ctrl+\) 
	signal(SIGQUIT, handle_sig_quit);
}

/*	Ctrl+\
	131	*/
void	handle_sig_quit(int n)
{
	if (n == SIGQUIT)
		ft_printf("Quit (core dumped)"); //program zorla bitirildiğinde bu yazı çıkar.
	write(1, "\n", STDERR_FILENO);//STDERR_FILENO sabiti aslında sadece sayıdır: 2. Burada 2'yi byte sayısı olarak kullanmış.
	// terminalde yeni bir satır açıyor eadline'da bozulma olmasın diye yapılıyor.
	rl_on_new_line();
/* 	Terminalin giriş satırını yeni satıra kaydırır ama içeriği silmez.
 Mesela Ctrl+C bastın diyelim, readline komutu yarım kaldı. rl_on_new_line() çağrılmazsa terminal satırı kayabilir */.
}

/*	Ctrl+c	*/
void	handle_sig_int(int n)
/* Kullanıcı Ctrl+C’ye bastığında, komut satırı temizlenir ve shell düzgün şekilde yeniden gösterilir.
Bu, shell’in kullanıcıya “yeni bir satırdan başla” demesini sağlar. */
{
	if (n == SIGINT)
	{
		rl_replace_line("", 0); //Readline’ın mevcut satırını temizler (buffer’ı sıfırlar).
		//mevcut satırı "" ile (boş string) ile değiştirir 
		// 0 = clear_undo geri alma bilgisini temizlemez eğer 1 olsaydı kullanıcı satırı geri alamazdı
		// 0 olduğunda kullanıcı eski satırı geri alabilir (undo stack korunuz)
		rl_on_new_line();  //rl_on_new_line(); Readline’a yeni bir satıra geçildiğini bildirir (iç durumu günceller).
		//Readline kullanan bir shell’de, genellikle Ctrl+_ (Ctrl ve alt çizgi tuşu, yani Ctrl+Shift+-) tuş kombinasyonu ile undo yapılır.
		write(1, "\n", STDERR_FILENO);
		// ??? stderr_fileno yanlış değil mi 1 olması gerekmez mi?????
		rl_redisplay();
	}
}
