/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:11:13 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/16 13:55:24 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handle_ctrl_c(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_on_new_line(); //this tells to rl that a new line was output 
	rl_replace_line("", 0); // this literally replaces the line with empty one
	rl_redisplay(); //this redraw the prompt and the previous empty line
}

void	ft_setup_signals(void)
{
	//sigset_t mask;
	//sigemptyset(&mask) signal masks preparation for sigaction
	//sigaddset(&mask, SIGINT)
	//sigaction
	signal(SIGINT, ft_handle_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN); // suspending (fg, bg, &, jobs)
}
