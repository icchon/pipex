/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 15:48:19 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/24 14:37:15 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	end(void)
{
	if (!grobal_asttree(GET, NULL))
	{
		free_tokens(grobal_token(GET, NULL));
	}
	else
	{
		free_asttree(grobal_asttree(GET, NULL));
	}
	if (grobal_tmpfile(GET, NULL))
	{
		unlink(grobal_tmpfile(GET, NULL));
		free(grobal_tmpfile(GET, NULL));
	}
	free(grobal_bashinput(GET, NULL));
	exit(EXIT_SUCCESS);
	return ;
}

int	main(int argc, char *argv[], char **env)
{
	char		*input;
	t_token		*tokens;
	t_astnode	*tree;
	char		buff[BUFFER_SIZE];

	if (argc < 5)
		return (0);
	grobal_env(SET, env);
	input = translate_like_bash(&argv[1]);
	grobal_bashinput(SET, input);
	tokens = lexer((char *)input);
	grobal_token(SET, tokens);
	tree = parser(tokens);
	grobal_asttree(SET, tree);
	check_fds(tree);
	executer(tree);
	if (tree)
	{
		ft_bzero(buff, BUFFER_SIZE);
		read(STDIN_FILENO, buff, BUFFER_SIZE);
		ft_dprintf(STDOUT_FILENO, "%s", buff);
	}
	end();
	return (0);
}
