/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 18:33:12 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/08 01:34:50 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"


t_astnode * new_astnode(){
	t_astnode *node;
	node = (t_astnode *)malloc(sizeof(t_astnode));
	if(!node)
		return NULL;
	node->cmd = NULL;
	node->arg_cnt = 0;
	node->args = NULL;
	node->left = NULL;
	node->right = NULL;
	node->type = ASTND_UNDEFINED;
	node->redirects = NULL;
	return node;
}

void add_astnode(t_astnode **root, t_astnode *left, t_astnode *right){
	if(*root == NULL){
		*root = new_astnode();
		if(!*root)
			return;
	}
	(*root)->left = left;
	(*root)->right = right;
	return;
}

t_astnode *parse_cmd(t_token **token){
	if(!*token)
		return NULL;
	t_astnode *node;
	if(!*token || (*token)->type != TK_WORD)
		return NULL;
	node = new_astnode();
	node->type = ASTND_CMD;
	node->cmd = (*token);
	(*token) = (*token)->next;
	if(*token && ft_ismatch((*token)->type, 2, TK_REDIRECT_IN, TK_HERE_DOC)){
			(*token) = (*token)->next;
			if(*token == NULL)
				return NULL;
			node->redirects = (t_redirect *)(*token);
			if(node->redirects->next){
				node->redirects->next->prev = NULL;
			}
			node->redirects->next = NULL;
		}
		else if(ft_ismatch((*token)->type, 3, TK_REDIRECT_IN, TK_REDIRECT_IN, TK_REDIRECT_OUT_APPEND)){
			(*token) = (*token)->next;
			if(*token == NULL)
				return NULL;
			node->redirects = (t_redirect *)(*token);
			while(*token && ft_ismatch((*token)->type, 2, TK_INPUT_FILE, TK_OUTPUT_FILE)){
				(*token) = (*token)->next;
			}
			if(*token){
				(*token)->prev->next = NULL;
				(*token)->prev = NULL;
			}
		}
		else if((*token)->type == TK_WORD){
			node->args = (t_arg *)(*token);
			while(*token && (*token)->type == TK_WORD){
				*token = (*token)->next;
			}
			if(*token){
				(*token)->prev->next = NULL;
				(*token)->prev = NULL;
			}
		}
		else{
			return NULL;
		}
	return node;
}

t_astnode *parse_pipe(t_token **token){
	t_astnode *left;
	left = parse_cmd(token);
	if(!left)
		return NULL;
	while(*token && ((*token)->type == TK_PIPE)){
		t_astnode *root;
		t_astnode *right;
		
		right = parse_cmd(token);
		if(!right)
			return NULL;
		root = new_astnode();
		root->type = ASTND_PIPE;
		root->left = left;
		root->right = right;
		left = root;
	}
	return left;
}



t_astnode *parse_or_and(t_token **token){
	t_astnode *left;

	left = parse_pipe(token);
	if(!left){
		printf("parse_or_and : left is NULL\n");
		return NULL;
	}
		
	while(*token && ((*token)->type == TK_OR || (*token)->type == TK_AND)){
		t_astnode *root;
		t_astnode *right;
		
		right = parse_pipe(token);
		if(!right)
			return NULL;
		root = new_astnode();
		if((*token)->type == TK_OR){
			root->type = ASTND_OR;
		}
		else {
			root->type = ASTND_AND;
		}
		root->left = left;
		root->right = right;
		left = root;
	}
	return left;
}




void print_ast_node(t_astnode *node, int depth) {
    if (!node)
        return;

    // インデントを出力
    for (int i = 0; i < depth; i++)
        printf("  ");

    // ノードの種類に応じて情報を出力
    switch (node->type) {
        case ASTND_PIPE:
            printf("PIPE\n");
            break;
        case ASTND_OR:
            printf("OR\n");
            break;
        case ASTND_AND:
            printf("AND\n");
            break;
        case ASTND_CMD:
            printf("CMD: %s\n", node->cmd->data);
            // 引数の出力
            t_arg *arg = node->args;
            while (arg) {
                for (int i = 0; i < depth + 1; i++)
                    printf("  ");
                printf("ARG: %s\n", arg->data);
                arg = arg->next;
            }
            // リダイレクトの出力
            t_redirect *red = node->redirects;
            while (red) {
                for (int i = 0; i < depth + 1; i++)
                    printf("  ");
                printf("REDIRECT(%d): %s\n", red->type, red->data);
                red = red->next;
            }
            break;
        default:
            printf("UNDEFINED\n");
    }

    // 子ノードを再帰的に出力
    print_ast_node(node->left, depth + 1);
    print_ast_node(node->right, depth + 1);
}

void print_ast(t_astnode *root) {
    printf("AST Structure:\n");
    print_ast_node(root, 0);
}


t_astnode *parser(t_token *token){	
	t_astnode *ast_tree;
	ast_tree = parse_or_and(&token);
	print_ast(ast_tree);
	return ast_tree;
	
}