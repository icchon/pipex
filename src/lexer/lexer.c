/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:40:34 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/13 17:58:48 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	init_lexer_status(int status[ASCII_SIZE])
{
	status[NORMAL] = 1;
	status[IN_DOUBLE_QUOT] = 0;
	status[IN_SINGLE_QAOT] = 0;
}

static void	update_lexer_status(int status[ASCII_SIZE], t_lexer_status type)
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

static void	add_token(char *line, size_t *i, t_token **head, size_t len)
{
	char	*data;
	t_token	*token;

	data = xsafe(end, (t_fvoid)ft_substr, 3, &line[*i], 0, len);
	token = new_token(TK_UNDEFINED, data);
	if (!token)
	{
		free(data);
		end();
	}
	addback_token(head, token);
	*i += len;
}

static int	process_in_normal_status(int *status, char *line, size_t *i,
		t_token **head)
{
	int		offset;
	char	*data;

	while (ft_isspace(line[*i]))
		(*i)++;
	offset = ft_calc_next_str(&line[*i], " <>|\"\'");
	if (offset > 0)
		add_token(line, i, head, offset);
	else
	{
		if (line[*i] == '\'' || line[*i] == '\"')
			return (update_lexer_status(status, (t_lexer_status)line[*i]), 1);
		if (ft_strncmp(&line[*i], "<<", 2) * ft_strncmp(&line[*i], ">>", 2)
			* ft_strncmp(&line[*i], "||", 2) * ft_strncmp(&line[*i], "&&",
				2) == 0)
			return (add_token(line, i, head, 2), 1);
		data = xsafe(end, (t_fvoid)ft_chr_to_str, 1, line[*i]);
		addback_token(head, new_token(TK_UNDEFINED, data));
		(*i)++;
	}
	return (0);
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
			process_in_normal_status(status, line, &i, &head);
		else
		{
			offset = ft_calc_next_chr(&line[i + 1], line[i]);
			update_lexer_status(status, (t_lexer_status)line[i]);
			data = xsafe(end, (t_fvoid)ft_substr, 3, &line[i], 0, offset + 2);
			addback_token(&head, new_token(TK_UNDEFINED, data));
			i += (offset + 2);
		}
	}
	tokenizer(&head);
	return (head);
}
