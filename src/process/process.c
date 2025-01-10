/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:41:39 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/10 18:40:58 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int exec_command(t_astnode *node){
	int pipes[2];
	int status;
	t_redirect *redirect;
	redirect = node->redirects;
	int fd;
	

	
	pipe(pipes);
	
	pid_t pid = fork();
	if (pid == CHILD_PID)
	{	
		close(pipes[READ]);
		dup2(pipes[WRITE], STDOUT_FILENO);
		close(pipes[WRITE]);
		while(redirect){
			if(redirect->type == TK_INPUT_FILE){
				fd = open(redirect->data, O_RDONLY);
				dup2(fd, STDIN_FILENO);
			}
			else if(redirect->type == TK_OUTPUT_FILE){
				fd = open(redirect->data, O_WRONLY | O_TRUNC | O_CREAT, S_IRGRP | S_IROTH | S_IWUSR | S_IRUSR);
				dup2(fd, STDOUT_FILENO);
			}
			else if(redirect->type == TK_OUTPUT_FILE_APPEND){
				fd = open(redirect->data, O_WRONLY | O_APPEND | O_CREAT, S_IRGRP | S_IROTH | S_IWUSR | S_IRUSR);
				dup2(fd, STDOUT_FILENO);
			}
			close(fd);
			redirect = redirect->next;
		}
		int res = try_command(node->cmd->data, node->arg_strs, grobal_env(GET, NULL));
		if (!res)
		{
			if (is_command(node->cmd->data))
				ft_dprintf(2, "%s: command not found\n", node->cmd->data);
			else
				ft_dprintf(2, "bash: %s: No such file or directory\n",
					node->cmd->data);
		}
		exit(EXIT_FAILURE);
	}
	wait(&status);
	if (status != EXIT_SUCCESS)
		exit(EXIT_FAILURE);
	close(pipes[WRITE]);
	dup2(pipes[READ], STDIN_FILENO);
	close(pipes[READ]);
	
	return status;
}	

char **create_args(t_arg* args){
	char **out;
	t_arg* arg;
	size_t arg_len;
	arg_len = size_token(args);
	size_t i;
	i = 0;
	arg = args;
	out = ft_calloc(arg_len + 2, sizeof(char * ));
	out[0] = "(^o^)";
	while(arg){
		out[i+1] = arg->data;
		arg = arg->next;
	}
	return out;
}

void set_exec(t_astnode *node){
	if(!node)
		return;
	
	if(node->args){
		node->arg_cnt = size_token(node->args);
		node->arg_strs = create_args(node->args);
	}
	return ;
}

t_astnode * executer(t_astnode *root){
	t_astnode_type type;
	t_astnode *left;
	type = root->type;
	if(!root){
		return NULL;
	}
	if(type == ASTND_CMD){

		expander(root);
		set_exec(root);
		exec_command(root);
		return root;
	}
	else if(type == ASTND_PIPE){
		executer(root->left);
		executer(root->right);	
	}
	else{
		left = executer(root->left);
		if(type == ASTND_AND){
			if(left){
				executer(root->right);
			}
		}
		else if(type == ASTND_OR){
			if(!left){
				executer(root->right);
			}
		}
		else{
			perror(NULL);
		}
	}
	return NULL;
}
