/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 18:33:12 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/13 21:50:46 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_astnode	*parser(t_token *token)
{
	t_astnode	*ast_tree;

	ast_tree = parse_or_and(&token);
	exec_heredoc(ast_tree);
	return (ast_tree);
}
