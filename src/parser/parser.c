/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 18:33:12 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/09 15:58:37 by kaisobe          ###   ########.fr       */
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
	node->arg_strs = NULL;
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
	if(!*token){

		return NULL;
	}
		
	t_astnode *node;
		
	
	t_token *redirects;
	t_token *args;
	t_token *to_add;
	node = new_astnode();
	redirects = NULL;
	args = NULL;
	while(*token && ft_ismatch((*token)->type, 5, TK_WORD, TK_REDIRECT_IN, TK_HERE_DOC, TK_REDIRECT_OUT, TK_REDIRECT_OUT_APPEND)){
		if(*token && ft_ismatch((*token)->type, 4, TK_REDIRECT_IN, TK_HERE_DOC, TK_REDIRECT_OUT, TK_REDIRECT_OUT_APPEND)){
			(*token) = (*token)->next;
			if(*token == NULL){
				return NULL;
			}
			to_add = (*token);
			(*token) = (*token)->next;
			addback_token(&redirects, to_add);
			// printf("redirects : ");
			// print_tokens(redirects);
			// printf("-----------------------\n");
			
		}
		else if((*token)->type == TK_WORD){
			if(node->type == ASTND_UNDEFINED){
				node->type = ASTND_CMD;
				to_add = (*token);
				(*token) = (*token)->next;
				addback_token(&(node->cmd), to_add);
			}
			else{
				to_add = (*token);
				(*token) = (*token)->next;
				addback_token(&args, to_add);
				// printf("args : ");
				// print_tokens(args);
				// printf("-----------------------\n");
			}
		}
	}
	node->args = args;
	node->redirects = redirects;
	// printf("---------------------\n");
	// printf("cmd : %s\n", node->cmd->data);
	// printf("args : ");
	// print_tokens((t_token *)node->args);
	// print_tokens((t_token *)node->redirects);
	// printf("---------------------\n");
	return node;
}

t_astnode *parse_pipe(t_token **token){
	t_astnode *left;
	left = parse_cmd(token);
	if(!left){
		return NULL;
	}
		
	while(*token && ((*token)->type == TK_PIPE)){
		t_astnode *root;
		t_astnode *right;
		root = new_astnode();
		root->type = ASTND_PIPE;
		(*token) = (*token)->next;
		right = parse_cmd(token);
		if(!right){
			return NULL;
		}
			
		
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

		return NULL;
	}
		
	while(*token && ((*token)->type == TK_OR || (*token)->type == TK_AND)){
		t_astnode *root;
		t_astnode *right;
		root = new_astnode();
		if((*token)->type == TK_OR){
			root->type = ASTND_OR;
		}
		else {
			root->type = ASTND_AND;
		}
		(*token) = (*token)->next;
		right = parse_pipe(token);

		if(!right){
	
			return left;
		}
		root->left = left;
		root->right = right;
		left = root;
	}
	return left;
}

void print_ast_node(t_astnode *node, int depth) {
    if (!node){
		return;
	}
    for (int i = 0; i < depth; i++)
        printf("  ");

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
            t_arg *arg = node->args;
            while (arg) {
                for (int i = 0; i < depth + 1; i++)
                    printf("  ");
                printf("ARG: %s\n", arg->data);
                arg = arg->next;
            }
            t_redirect *red = node->redirects;
            while (red) {
                for (int i = 0; i < depth + 1; i++)
                    printf("  ");
				if(red->type == TK_INPUT_FILE)
               		printf("REDIRECT < %s\n",red->data);
				else if(red->type == TK_OUTPUT_FILE)
					printf("REDIRECT > %s\n",red->data);
				else if(red->type == TK_LIMITER)
					printf("HERE_DOC LIMITER : %s\n",red->data);
				else 
					printf("REDIRECT >> %s\n",red->data);
                red = red->next;
            }
            break;
        default:
            printf("UNDEFINED\n");
    }

    print_ast_node(node->left, depth + 1);
    print_ast_node(node->right, depth + 1);
}

void print_ast(t_astnode *root) {
    printf("AST Structure:\n");
    print_ast_node(root, 0);
}

void exec_heredoc(t_astnode *node){
	if(!node){
		return;
	}
	t_redirect * redirect;
	char *limiter;
	
	redirect = node->redirects;
	while(redirect){
		if(redirect->type == TK_LIMITER){
			limiter = redirect->data;
			redirect->data = process_heredoc(limiter);
			redirect->data = replace_env_vars(redirect->data, grobal_env(GET, NULL));
			redirect->type = TK_INPUT_FILE;
		}
		redirect = redirect->next;
	}
	exec_heredoc(node->left);
	exec_heredoc(node->right);
}


t_astnode *parser(t_token *token){	
	t_astnode *ast_tree;
	ast_tree = parse_or_and(&token);
	exec_heredoc(ast_tree);
	return ast_tree;
	
}