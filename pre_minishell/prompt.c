/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 21:32:14 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/16 12:10:30 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_fancy_address(t_mini *ms)
{
	char	*tmp;
	char	*res;

	if (!ft_strncmp(ms->cwd, "/home", 5)
		|| !ft_strncmp(ms->cwd + 6, ms->username, ft_strlen(ms->username)))
	{
		tmp = ft_substr(ms->cwd, 14, ft_strlen(ms->cwd));
		if (!tmp)
			ft_clean_str(ms, 1);
		res = ft_strjoin("~", tmp);
		free(tmp);
		tmp = NULL;
	}
	else
		res = ft_strdup(ms->cwd);
	if (!res)
		ft_clean_str(ms, 1);
	return (res);
}

char	*ft_finish_prompt(t_mini *ms, char *cwd)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(ms->fullname, cwd);
	free(cwd);
	if (!tmp)
		ft_clean_str(ms, 1);
	res = ft_strjoin(tmp, "$ ");
	free(tmp);
	if (!res)
		ft_clean_str(ms, 1);
	return (res);
}

char	*ft_add_colors(t_mini *ms, char *prompt)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(ms->color_start, prompt);
	free(prompt);
	if (!tmp)
		ft_clean_str(ms, 1);
	res = ft_strjoin(tmp, ms->color_end);
	free(tmp);
	if (!res)
		ft_clean_str(ms, 1);
	return (res);
}

char	*ft_make_prompt(t_mini *ms)
{
	char	*fancy_address;
	char	*prompt;
	char	*colored_prompt;

	if (getcwd(ms->cwd, BUFSIZ) == NULL)
		ft_clean_str(ms, 1);
	fancy_address = ft_get_fancy_address(ms);
	prompt = ft_finish_prompt(ms, fancy_address);
	colored_prompt = ft_add_colors(ms, prompt);
	return (colored_prompt);
}
