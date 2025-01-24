/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 20:38:28 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/24 14:08:11 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	set_tokens(t_token *current, t_token_type type1, t_token *next,
		t_token_type type2)
{
	if (current)
	{
		current->type = type1;
	}
	if (next)
	{
		next->type = type2;
	}
	return ;
}

static void	set_type(t_token **current, t_token **next, t_token **prev)
{
	if ((*current)->type != TK_UNDEFINED)
	{
		*current = *next;
		*next = (*current)->next;
		*prev = (*current)->prev;
		return ;
	}
	else if (ft_isequal((*current)->data, "<<"))
		set_tokens(*current, TK_HERE_DOC, *next, TK_LIMITER);
	else if (ft_isequal((*current)->data, ">>"))
		set_tokens(*current, TK_REDIRECT_OUT_APPEND, *next,
			TK_OUTPUT_FILE_APPEND);
	else if (ft_isequal((*current)->data, "||"))
		set_tokens(*current, TK_OR, NULL, TK_UNDEFINED);
	else if (ft_isequal((*current)->data, "&&"))
		set_tokens(*current, TK_AND, NULL, TK_UNDEFINED);
	else if (ft_isequal((*current)->data, ">"))
		set_tokens(*current, TK_REDIRECT_OUT, *next, TK_OUTPUT_FILE);
	else if (ft_isequal((*current)->data, "<"))
		set_tokens(*current, TK_REDIRECT_IN, *next, TK_INPUT_FILE);
	else if (ft_isequal((*current)->data, "|"))
		set_tokens(*current, TK_PIPE, *next, TK_WORD);
	else
		set_tokens(*current, TK_WORD, NULL, TK_UNDEFINED);
}

int	tokenizer(t_token **token_ptr)
{
	t_token	*current;
	t_token	*next;
	t_token	*prev;

	current = head_token(*token_ptr);
	if (!current)
		return (0);
	next = current->next;
	while (next)
	{
		set_type(&current, &next, &prev);
	}
	if (current->type == TK_UNDEFINED)
		set_tokens(current, TK_WORD, NULL, TK_UNDEFINED);
	return (0);
}

// void	print_token(t_token *token)
// {
// 	int	type;

// 	if (!token)
// 	{
// 		printf("null\n");
// 		return ;
// 	}
// 	printf("data : %s\n", token->data);
// 	type = (int)token->type;
// 	if (type == TK_WORD)
// 		printf("type : TK_WORD\n");
// 	else if (type == TK_PIPE)
// 		printf("type : TK_PIPE\n");
// 	else if (type == TK_HERE_DOC)
// 		printf("type : TK_HERE_DOC\n");
// 	else if (type == TK_OR)
// 		printf("type : TK_OR\n");
// 	else if (type == TK_AND)
// 		printf("type : TK_AND\n");
// 	else if (type == TK_LIMITER)
// 		printf("type : TK_LIMITER\n");
// 	else if (type == TK_INPUT_FILE)
// 		printf("type : TK_INPUT_FILE\n");
// 	else if (type == TK_OUTPUT_FILE)
// 		printf("type : TK_OUTPUT_FILE\n");
// 	else if (type == TK_OUTPUT_FILE_APPEND)
// 		printf("type : TK_OUTPUT_FILE_APPEND\n");
// 	else if (type == TK_REDIRECT_OUT)
// 		printf("type : TK_REDIRECT_OUT\n");
// 	else if (type == TK_REDIRECT_OUT_APPEND)
// 		printf("type : TK_REDIRECT_OUT_APPEND\n");
// 	else if (type == TK_REDIRECT_IN)
// 		printf("type : TK_REDIRECT_IN\n");
// 	else if (type == TK_UNDEFINED)
// 		printf("type == TK_UNDEFINED\n");
// 	else
// 	{
// 		printf("type : error\n");
// 	}
// }

// void	print_tokens(t_token *token)
// {
// 	t_token	*node;

// 	node = token;
// 	if (!node)
// 		printf("null");
// 	while (node)
// 	{
// 		print_token(node);
// 		node = node->next;
// 	}
// 	return ;
// }
