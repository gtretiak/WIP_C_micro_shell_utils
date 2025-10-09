/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 21:33:29 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/16 19:05:23 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_welcoming(void)
{
	printf("%s"
		"╦ ╦┌─┐┬  ┌─┐┌─┐┌┬┐┌─┐  ┌┬┐┌─┐  ┌┬┐┬ ┬\n"
		"║║║├┤ │  │  │ ││││├┤    │ │ │  │││└┬┘\n"
		"╚╩╝└─┘┴─┘└─┘└─┘┴ ┴└─┘   ┴ └─┘  ┴ ┴ ┴ \n\n"
		"███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     \n"
		"████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     \n"
		"██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     \n"
		"██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     \n"
		"██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗\n"
		"╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n"
		"%s", GREEN, RESET);
}

void	ft_init_minishell(t_mini *ms, char **env)
{
	ms->cwd = NULL;
	ms->hostname = NULL;
	ms->prompt = NULL;
	ms->hostname = NULL;
	ms->fullname = NULL;
	ms->input = NULL;
	ms->tokens = NULL;
	ms->color_start = GREEN;
	ms->color_end = RESET;
	ms->bin_dir = getenv("PATH");
	ms->username = getenv("USER");
	ms->session = getenv("SESSION_MANAGER");
	ms->cwd = malloc(sizeof(char) * BUFSIZ);
	if (!ms->cwd)
		exit(EXIT_FAILURE);
	ms->hostname = ft_substr(ms->session, 6, 6);
	if (!ms->hostname)
		ft_clean_str(ms, 1);
	ft_get_full_name(ms);
	ft_get_env(ms, env);
}

void	ft_get_full_name(t_mini *ms)
{
	char	*tmp1;
	char	*tmp2;

	tmp1 = ft_strjoin(ms->username, "@");
	if (!tmp1)
		ft_clean_str(ms, 1);
	tmp2 = ft_strjoin(tmp1, ms->hostname);
	free(tmp1);
	if (!tmp2)
		ft_clean_str(ms, 1);
	ms->fullname = ft_strjoin(tmp2, ":");
	free(tmp2);
	if (!ms->fullname)
		ft_clean_str(ms, 1);
}

void	ft_get_env(t_mini *ms, char **env)
{
}
