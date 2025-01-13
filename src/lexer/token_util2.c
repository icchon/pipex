/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_util2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 11:50:39 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/13 20:42:09 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

size_t	size_token(t_token *token)
{
	t_token	*node;
	size_t	cnt;

	node = token;
	cnt = 0;
	while (node)
	{
		node = node->next;
		cnt++;
	}
	return (cnt);
}

void	delone_token(t_token *token, void (*del)(void *))
{
	if (!token)
		return ;
	del(token->data);
	free(token);
	return ;
}

void	cut_token(t_token **token, t_token *to_delete)
{
	t_token	*prev;
	t_token	*next;

	if (!to_delete)
		return ;
	prev = to_delete->prev;
	next = to_delete->next;
	if (prev)
		prev->next = NULL;
	if (next)
		next->prev = NULL;
	delone_token(to_delete, free);
	if (!prev && !next)
		*token = NULL;
	return ;
}

void	free_tokens(t_token *token)
{
	t_token	*node;
	t_token	*next;

	node = head_token(token);
	while (node)
	{
		next = node->next;
		delone_token(node, free);
		node = next;
	}
	return ;
}
