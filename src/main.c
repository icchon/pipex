/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 15:48:19 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/10 18:40:09 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char *argv[], char **env)
{
	char	*input;
	t_token *tokens;
	t_astnode *astnode;
	char *	line = "ls -l | wc  >> output";

	(void)env;
	(void)argc;
	// if (argc < 5)
	// 	return (0);
	grobal_env(SET, env);
	input = translate_like_bash(&argv[1]);
	tokens = lexer(line);
	//print_tokens(tokens);
	astnode = parser(tokens);
	print_ast(astnode);
	executer(astnode);
	// char buff[BUFFER_SIZE];
	// ft_bzero(buff, BUFFER_SIZE);
	// read(STDIN_FILENO, buff, BUFFER_SIZE);
	// ft_dprintf(STDOUT_FILENO, "%s", buff);
	return (0);
}
