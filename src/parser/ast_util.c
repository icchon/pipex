/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:28:59 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/24 14:08:30 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_astnode	*new_astnode(void)
{
	t_astnode	*node;

	node = (t_astnode *)malloc(sizeof(t_astnode));
	if (!node)
		return (NULL);
	node->cmd = NULL;
	node->arg_cnt = 0;
	node->args = NULL;
	node->left = NULL;
	node->right = NULL;
	node->type = ASTND_UNDEFINED;
	node->redirects = NULL;
	node->arg_strs = NULL;
	return (node);
}

void	free_astnode(t_astnode *node)
{
	if (!node)
		return ;
	free_tokens(node->cmd);
	free_tokens(node->args);
	free_tokens(node->redirects);
	free(node->arg_strs);
	free(node);
}

void	free_asttree(t_astnode *root)
{
	t_astnode	*left;
	t_astnode	*right;

	if (!root)
		return ;
	left = root->left;
	right = root->right;
	free_astnode(root);
	if (left)
	{
		free_asttree(left);
	}
	if (right)
		free_asttree(right);
	return ;
}

void	add_astnode(t_astnode **root, t_astnode *left, t_astnode *right)
{
	if (*root == NULL)
	{
		*root = new_astnode();
		if (!*root)
			return ;
	}
	(*root)->left = left;
	(*root)->right = right;
	return ;
}

// static void	print_ast_node(t_astnode *node, int depth)
// {
// 	t_arg		*arg;
// 	t_redirect	*red;
// 	int			i;

// 	if (!node)
// 	{
// 		return ;
// 	}
// 	i = 0;
// 	while (i < depth)
// 	{
// 		printf("  ");
// 		i++;
// 	}
// 	if (node->type == ASTND_PIPE)
// 		printf("PIPE\n");
// 	else if (node->type == ASTND_OR)
// 		printf("OR\n");
// 	else if (node->type == ASTND_AND)
// 		printf("AND\n");
// 	else if (node->type == ASTND_CMD)
// 	{
// 		printf("CMD: %s\n", node->cmd->data);
// 		arg = node->args;
// 		while (arg)
// 		{
// 			i = 0;
// 			while (i < depth)
// 			{
// 				printf("  ");
// 				i++;
// 			}
// 			printf("ARG: %s\n", arg->data);
// 			arg = arg->next;
// 		}
// 		red = node->redirects;
// 		while (red)
// 		{
// 			i = 0;
// 			while (i < depth)
// 			{
// 				printf("  ");
// 				i++;
// 			}
// 			if (red->type == TK_INPUT_FILE)
// 				printf("REDIRECT < %s\n", red->data);
// 			else if (red->type == TK_OUTPUT_FILE)
// 				printf("REDIRECT > %s\n", red->data);
// 			else if (red->type == TK_LIMITER)
// 				printf("HERE_DOC LIMITER : %s\n", red->data);
// 			else
// 				printf("REDIRECT >> %s\n", red->data);
// 			red = red->next;
// 		}
// 	}
// 	else
// 		printf("UNDEFINED\n");
// 	print_ast_node(node->left, depth + 1);
// 	print_ast_node(node->right, depth + 1);
// }

// void	print_ast(t_astnode *root)
// {
// 	printf("AST Structure: ");
// 	if (!root)
// 		printf("NULL\n");
// 	print_ast_node(root, 0);
// }
