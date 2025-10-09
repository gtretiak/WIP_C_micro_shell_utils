/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:53:29 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/16 18:57:57 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_mini		ms;
	t_tokens	*tmp;

	(void)ac;
	(void)av;
	ft_setup_signals();
	ft_welcoming();
	ft_init_minishell(&ms, env);
	while (1)
	{
		ms.prompt = ft_make_prompt(&ms);
		ms.input = readline((const char *)ms.prompt);
		if (!ms.input)
		{
			write(1, "exit\n", 5);
			break ;
		}
		ft_tokenization(&ms);
		tmp = ms.tokens;
		while (tmp)
		{
			if (!ft_strncmp(tmp->line, "exit", 5))
				ft_exit(tmp);
			else if (!ft_strncmp(tmp->line, "pwd", 4))
				ft_pwd(&ms);
			else
				printf("%s\n", tmp->line);
			tmp = tmp->next;
		}
		//ft_parsing();
		// if heredoc, listen until EOF, update the input, then add in history TODO
		add_history(ms.input);
		free(ms.prompt);
		free(ms.input);
		ft_free_lists(&ms.tokens);
	}
	ft_clean_str(&ms, 0);
	return (EXIT_SUCCESS);
}
//TODO
//built-in:
//env with no options and do args
//unset with args, including func, but no options
//export with args=value, but no options
//echo -n + args (strings or $VARS)
//cd with no options, and absolute(cd /home/user/... or cd ~) or relative (cd ., cd ..) + bonus (cd -)
//
//chdir
