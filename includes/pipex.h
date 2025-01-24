/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 15:48:29 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/24 12:39:08 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define INPUT 0
# define OUTPUT 1
# define READ 0
# define WRITE 1
# define CHILD_PID 0
# define PARENT_PID 1
# define GET 0
# define SET 1

typedef enum e_token_type
{
	TK_WORD,
	TK_PIPE,
	TK_OR,
	TK_AND,
	TK_HERE_DOC,
	TK_LIMITER,
	TK_INPUT_FILE,
	TK_OUTPUT_FILE,
	TK_OUTPUT_FILE_APPEND,
	TK_REDIRECT_OUT = '>',
	TK_REDIRECT_OUT_APPEND,
	TK_REDIRECT_IN = '<',
	TK_UNDEFINED,
}						t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*data;
	struct s_token		*prev;
	struct s_token		*next;
}						t_token;

typedef enum e_lexer_status
{
	IN_SINGLE_QAOT = '\'',
	IN_DOUBLE_QUOT = '\"',
	NORMAL = '\0',
}						t_lexer_status;

typedef enum e_astnode_type
{
	ASTND_PIPE,
	ASTND_OR,
	ASTND_AND,
	ASTND_CMD,
	ASTND_UNDEFINED,
}						t_astnode_type;

typedef t_token			t_redirect;
typedef t_token			t_arg;
typedef t_token			t_cmd;
typedef void			*(*t_fvoid)(void *, ...);

typedef struct s_astnode
{
	t_astnode_type		type;
	t_cmd				*cmd;
	char				**arg_strs;
	t_arg				*args;
	t_redirect			*redirects;
	size_t				arg_cnt;
	struct s_astnode	*left;
	struct s_astnode	*right;
}						t_astnode;

t_astnode				*executer(t_astnode *root);
void					exec_heredoc(t_astnode *node);
int						exec_command(t_astnode *node);
void					expander(t_astnode *node);
char					**grobal_env(int get_or_set, char **env);
t_token					*lexer(char *line);
t_token					*new_token(t_token_type type, char *data);
t_token					*last_token(t_token *token);
void					addback_token(t_token **token, t_token *new);
void					addfront_token(t_token **token, t_token *new);
t_token					*head_token(t_token *token);
size_t					size_token(t_token *token);
void					delone_token(t_token *token, void (*del)(void *));
void					cut_token(t_token **token, t_token *to_delete);
void					print_tokens(t_token *token);
void					print_token(t_token *token);
int						tokenizer(t_token **token_ptr);
t_astnode				*new_astnode(void);
void					add_astnode(t_astnode **root, t_astnode *left,
							t_astnode *right);
void					print_ast(t_astnode *root);
t_astnode				*parser(t_token *token);
char					*translate_like_bash(char **args);
void					ft_free_arrs(int **arr);
void					ft_free_str3(char ***strs);
int						is_directory(char *str);
int						is_command(char *str);
char					*replace_env_vars(char *str, char **env);
void					free_astnode(t_astnode *root);
void					free_tokens(t_token *token);
void					free_asttree(t_astnode *root);
void					*xsafe(void (*g)(void), t_fvoid f, int argn, ...);
t_astnode				*grobal_asttree(int get_or_set, t_astnode *tree);
char					*grobal_bashinput(int get_or_set, char *bashinput);
void					end(void);
t_token					*grobal_token(int get_or_set, t_token *token);
t_astnode				*parse_or_and(t_token **token);
char					*grobal_tmpfile(int get_or_set, char *tmpfile);
void					*xsafe(void (*g)(void), t_fvoid f, int argn, ...);
void					check_fds(t_astnode *tree);

#endif