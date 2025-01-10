/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:40:34 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/09 13:42:50 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_lexer_status(int status[ASCII_SIZE])
{
	status[NORMAL] = 1;
	status[IN_DOUBLE_QUOT] = 0;
	status[IN_SINGLE_QAOT] = 0;
}

void	update_lexer_status(int status[ASCII_SIZE], t_lexer_status type)
{
	status[type] = !status[type];
	if (type == NORMAL)
	{
		if (status[type])
		{
			init_lexer_status(status);
		}
	}
	else if (type == IN_DOUBLE_QUOT || type == IN_SINGLE_QAOT)
	{
		if (!status[IN_DOUBLE_QUOT] && !status[IN_SINGLE_QAOT])
		{
			status[NORMAL] = 1;
		}
		else
		{
			status[NORMAL] = 0;
		}
	}
}

int	tokenizer(t_token **token_ptr)
{
	t_token	*current;
	t_token	*next;
	t_token *prev;

	current = head_token(*token_ptr);
	next = current->next;
	while (next)
	{
		if (current->type != TK_UNDEFINED)
		{
			current = next;
			next = current->next;
			prev = current->prev;
			continue ;
		}
		else if(ft_isequal(current->data, "<<")){
			current->type = TK_HERE_DOC;
			next->type = TK_LIMITER;
		}
		else if(ft_isequal(current->data, ">>")){
			current->type = TK_REDIRECT_OUT_APPEND;
			next->type = TK_OUTPUT_FILE_APPEND;
		}
		else if(ft_isequal(current->data, "||")){
			current->type = TK_OR;
		}
		else if(ft_isequal(current->data, "&&")){
			current->type = TK_AND;
		}
		else if (ft_isequal(current->data, ">"))
		{
			current->type = TK_REDIRECT_OUT;
			next->type = TK_OUTPUT_FILE;
		}
		else if (ft_isequal(current->data, "<"))
		{
			current->type = TK_REDIRECT_IN;
			next->type = TK_INPUT_FILE;
		}
		else if (ft_isequal(current->data, "|"))
		{
			current->type = TK_PIPE;
			next->type = TK_WORD;
		}
		else{
			current->type = TK_WORD;
		}
	}
	if(current->type == TK_UNDEFINED){
		current->type = TK_WORD;
	}
	return 0;
}


void print_token(t_token *token){
	if(!token){
		printf("null\n");
		return;
	}
	printf("data : %s\n", token->data);
	int type;
	type = (int)token->type;
	if(type == TK_WORD)
		printf("type : TK_WORD\n");
	else if(type == TK_PIPE)
		printf("type : TK_PIPE\n");
	else if(type == TK_HERE_DOC)
		printf("type : TK_HERE_DOC\n");
	else if(type == TK_OR)
		printf("type : TK_OR\n");
	else if(type == TK_AND)
		printf("type : TK_AND\n");
	else if(type == TK_LIMITER)
		printf("type : TK_LIMITER\n");
	else if(type == TK_INPUT_FILE)
		printf("type : TK_INPUT_FILE\n");
	else if(type == TK_OUTPUT_FILE)
		printf("type : TK_OUTPUT_FILE\n");
	else if(type == TK_OUTPUT_FILE_APPEND)
		printf("type : TK_OUTPUT_FILE_APPEND\n");
	else if(type == TK_REDIRECT_OUT)
		printf("type : TK_REDIRECT_OUT\n");
	else if(type == TK_REDIRECT_OUT_APPEND)
		printf("type : TK_REDIRECT_OUT_APPEND\n");
	else if(type == TK_REDIRECT_IN)
		printf("type : TK_REDIRECT_IN\n");
	else if(type == TK_UNDEFINED)
		printf("type == TK_UNDEFINED\n");
	else {
		printf("type : error\n");
	}
}

void print_tokens(t_token *token){
	t_token *node;
	node = token;
	while(node){
		print_token(node);
		node = node->next;
	}
	return;
}

t_token	*lexer(char *line)
{
	t_token	*head;
	char	*data;
	size_t	i;
	int		offset;
	int		status[ASCII_SIZE];

	init_lexer_status(status);

	head = NULL;
	i = 0;
	while (line[i])
	{
		if (status[NORMAL])
		{
			while (ft_isspace(line[i]))
				i++;
			offset = ft_calc_next_str(&line[i], " <>|\"\'");
			if (offset > 0)
			{
				data = ft_substr(&line[i], 0, offset);
				addback_token(&head, new_token(TK_UNDEFINED, data));
				//printf("add data : [%s]\n", data);
				i += offset;
			}
			else
			{
				if (line[i] == '\'' || line[i] == '\"')
				{
					update_lexer_status(status, (t_lexer_status)line[i]);
					continue ;
				}
				if (((line[i] == '<') && (line[i + 1] == '<'))
					|| ((line[i] == '>') && (line[i + 1] == '>')) || ((line[i] == '|') && (line[i + 1] == '|')) || ((line[i] == '&') && (line[i + 1] == '&')))
				{
					data = ft_substr(&line[i], 0, 2);
					addback_token(&head, new_token(TK_UNDEFINED, data));
					//printf("add data : [%s]\n", data);
					i += 2;
					continue;
				}
				data = ft_chr_to_str(line[i]);
				addback_token(&head, new_token(TK_UNDEFINED, data));
				//printf("add data : [%s]\n", data);
				i++;
			}
		}
		else
		{
			offset = ft_calc_next_chr(&line[i + 1], line[i]);
			update_lexer_status(status, (t_lexer_status)line[i]);
			data = ft_substr(&line[i], 0, offset + 2);
			addback_token(&head, new_token(TK_UNDEFINED, data));
			//printf("add data : [%s]\n", data);
			i += (offset + 2);
		}
		//printf("----------------------\n");
	}
	tokenizer(&head);
	return (head);
}

