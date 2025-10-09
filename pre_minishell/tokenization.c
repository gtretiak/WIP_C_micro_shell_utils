/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 10:33:49 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/16 17:32:24 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tokens	*ft_find_last(t_tokens *head)
{
	while (head->next)
		head = head->next;
	return (head);
}

int	ft_append_token(t_tokens **tokens, char *s)
{
	t_tokens	*node;
	t_tokens	*last;

	node = malloc(sizeof(t_tokens));
	if (!node)
		return (EXIT_FAILURE);
	node->next = NULL;
	node->line = ft_strdup(s);
	if (!node->line)
		return (EXIT_FAILURE);
	if (!(*tokens))
	{
		*tokens = node;
		node->prev = NULL;
	}
	else
	{
		last = ft_find_last(*tokens);
		last->next = node;
		node->prev = last;
	}
	return (EXIT_SUCCESS);
}

void	ft_tokenization(t_mini *ms)
{
	char	**arr;
	int		i;

	i = 0;
	arr = ft_split_whitespace(ms->input);
	if (!arr)
		ft_clean_str(ms, 1);
	while (arr[i])
	{
		if (ft_append_token(&ms->tokens, arr[i]))
		{
			ft_free_lists(&ms->tokens);
			ft_free_array_str(arr);
			ft_clean_str(ms, 1);
		}
		i++;
	}
	ft_free_array_str(arr);
}
