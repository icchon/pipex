/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 11:50:39 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/09 14:21:44 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_token	*new_token(t_token_type type, char *data)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->data = data;
	token->prev = NULL;
	token->next = NULL;
	return (token);
}

t_token	*last_token(t_token *token)
{
	t_token	*node;

	if (token == NULL)
		return (token);
	node = token;
	if (node->next == NULL)
	{
	}
	while (node->next != NULL)
	{
		node = node->next;
	}
	return (node);
}

void	addback_token(t_token **token, t_token *new)
{
	t_token	*last_node;

	last_node = last_token(*token);
	new->next = NULL;
	new->prev = last_node;
	if (last_node == NULL)
	{
		*token = new;
		return ;
	}
	last_node->next = new;
	return ;
}

void	addfront_token(t_token **token, t_token *new)
{
	if (new == NULL)
		return ;
	new->next = *token;
	new->prev = NULL;
	if (*token != NULL)
		(*token)->prev = new;
	*token = new;
}

t_token	*head_token(t_token *token)
{
	t_token	*node;

	node = token;
	if (node == NULL)
		return (NULL);
	while (node->prev != NULL)
	{
		node = node->prev;
	}
	return (node);
}

size_t size_token(t_token *token){
	t_token *node;
	node = token;
	size_t cnt;
	cnt = 0;
	while(node){
		node = node->next;
		cnt++;
	}
	return cnt;
}

void	delone_token(t_token *token, void (*del)(void *))
{
	if (token == NULL)
		return ;
	del(token->data);
	free(token);
	return ;
}

void	delete_token(t_token **token, t_token *to_delete)
{
	t_token	*node;
	t_token	*prev;
	t_token	*next;

	node = head_token(*token);
	if (!node)
		return ;
	while (node)
	{
		if (node == to_delete)
		{
			prev = node->prev;
			next = node->next;
			if (prev)
				prev->next = next;
			if (next)
				next->prev = prev;
			delone_token(node, free);
			if (!prev && !next)
			{
				*token = NULL;
			}
			return ;
		}
		node = node->next;
	}
	return ;
}
