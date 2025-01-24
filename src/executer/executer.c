/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:45:25 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/24 12:15:02 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	**create_args(t_astnode *node)
{
	char	**out;
	t_arg	*arg;
	size_t	arg_len;
	size_t	i;

	if (!node)
		return (NULL);
	arg_len = size_token(node->args);
	i = 1;
	arg = node->args;
	out = (char **)malloc(sizeof(char *) * (arg_len + 2));
	if (!out)
		return (NULL);
	out[0] = node->cmd->data;
	while (arg)
	{
		out[i++] = arg->data;
		arg = arg->next;
	}
	out[arg_len + 1] = NULL;
	return (out);
}

static void	set_exec(t_astnode *node)
{
	if (!node)
		return ;
	if (node->args)
	{
		node->arg_cnt = size_token(node->args);
		node->arg_strs = xsafe(end, (t_fvoid)create_args, 1, node);
	}
	return ;
}

t_astnode	*executer(t_astnode *root)
{
	t_astnode_type	type;
	t_astnode		*left;

	if (!root)
		return (NULL);
	type = root->type;
	if (type == ASTND_CMD)
	{
		if (!(expander(root), set_exec(root), exec_command(root)))
			return (NULL);
		return (root);
	}
	else if (type == ASTND_PIPE)
		return (executer(root->left), executer(root->right), NULL);
	else
	{
		left = executer(root->left);
		if (type == ASTND_AND && left)
			executer(root->right);
		else if (type == ASTND_OR && !left)
			executer(root->right);
	}
	return (NULL);
}
