/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:53:06 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/16 17:28:13 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_clean_cwd(t_mini *ms)
{
	if (ms->cwd)
	{
		free(ms->cwd);
		ms->cwd = NULL;
	}
}

void	ft_clean_name(t_mini *ms)
{
	if (ms->hostname)
	{
		free(ms->hostname);
		ms->hostname = NULL;
	}
	if (ms->fullname)
	{
		free(ms->fullname);
		ms->fullname = NULL;
	}
}

void	ft_clean_str(t_mini *ms, int code)
{
	if (ms->cwd)
		ft_clean_cwd(ms);
	if (ms->hostname || ms->fullname)
		ft_clean_name(ms);
	if (ms->prompt)
	{
		free(ms->prompt);
		ms->prompt = NULL;
	}
	if (code)
		exit(EXIT_FAILURE);
}

void	ft_free_lists(t_tokens **tokens)
{
	t_tokens	*tmp;

	if (!tokens || !*tokens)
		return ;
	while (*tokens)
	{
		tmp = (*tokens)->next;
		free((*tokens)->line);
		free(*tokens);
		*tokens = tmp;
	}
	*tokens = NULL;
}
