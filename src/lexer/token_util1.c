/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_util1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:16:22 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/24 12:02:42 by kaisobe          ###   ########.fr       */
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
	while (node->next != NULL)
	{
		node = node->next;
	}
	return (node);
}

void	addback_token(t_token **token, t_token *new)
{
	t_token	*last_node;

	if (!new)
		return ;
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
