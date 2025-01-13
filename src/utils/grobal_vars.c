/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grobal_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:53:46 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/13 20:29:12 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**grobal_env(int get_or_set, char **env)
{
	static char	**g_env = NULL;

	if (get_or_set == GET)
	{
		return (g_env);
	}
	else
	{
		g_env = env;
	}
	return (g_env);
}

t_astnode	*grobal_asttree(int get_or_set, t_astnode *tree)
{
	static t_astnode	*g_tree = NULL;

	if (get_or_set == GET)
	{
		return (g_tree);
	}
	else
	{
		g_tree = tree;
	}
	return (g_tree);
}

char	*grobal_bashinput(int get_or_set, char *bashinput)
{
	static char	*g_bashinput = NULL;

	if (get_or_set == GET)
	{
		return (g_bashinput);
	}
	else
	{
		g_bashinput = bashinput;
	}
	return (g_bashinput);
}

t_token	*grobal_token(int get_or_set, t_token *token)
{
	static t_token	*g_token = NULL;

	if (get_or_set == GET)
	{
		return (g_token);
	}
	else
	{
		g_token = token;
	}
	return (g_token);
}

char	*grobal_tmpfile(int get_or_set, char *tmpfile)
{
	static char	*g_tmpfile = NULL;

	if (get_or_set == GET)
	{
		return (g_tmpfile);
	}
	else
	{
		g_tmpfile = tmpfile;
	}
	return (g_tmpfile);
}
