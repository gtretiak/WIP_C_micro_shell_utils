/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 17:53:15 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/16 18:50:26 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(t_tokens *exit_node)
{
	int	n;

	printf("exit\n");
	if (exit_node->next)
	{
		n = ft_atoi(exit_node->next->line);
		if (n >= 0 && n <= 256)
			exit(n);
		else
			exit(EXIT_FAILURE);
	}
	else
		exit(EXIT_SUCCESS);
}
