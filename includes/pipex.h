/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 15:48:29 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/08 00:21:05 by kaisobe          ###   ########.fr       */
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

typedef enum e_token_type
{
	TK_WORD,     // cat
	TK_PIPE,        // |
	TK_OR,//||
	TK_AND,//&&
	TK_HERE_DOC,    // <<
	TK_LIMITER,     // EOF
	TK_INPUT_FILE,  // input.txt
	TK_OUTPUT_FILE, // output.txt
	TK_REDIRECT_OUT = '>',
	TK_REDIRECT_OUT_APPEND, // >>
	TK_REDIRECT_IN = '<',
	TK_UNDEFINED,
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*data;
	struct s_token	*prev;
	struct s_token	*next;
}					t_token;

typedef struct s_command
{
	char			*binary_path;
	char			**args;
	size_t			args_len;
	int				pipes[2];
	int				io_fds[2];
}					t_command;

typedef enum e_lexer_status
{
	IN_SINGLE_QAOT = '\'',
	IN_DOUBLE_QUOT = '\"',
	NORMAL = '\0',
}					t_lexer_status;


typedef enum e_astnode_type{
	ASTND_PIPE,
	ASTND_OR,
	ASTND_AND,
	ASTND_CMD,
	ASTND_UNDEFINED,
} t_astnode_type;

typedef t_token t_redirect;
typedef t_token t_arg;
typedef t_token t_cmd;

typedef struct s_astnode{
	t_astnode_type type;
	t_cmd *cmd;
	t_arg *args;
	t_redirect *redirects;
	size_t arg_cnt;
	struct s_astnode *left;
	struct s_astnode *right;
} t_astnode;

t_token				*new_token(t_token_type type, char *data);
t_token				*last_token(t_token *token);
void				addback_token(t_token **token, t_token *new);
void				addfront_token(t_token **token, t_token *new);
char				*translate_like_bash(char **args);
int					std_pipe(t_vars *data, char **env);
t_token				*lexer(char *line);
char				**expand_args(char **args, char **env);
int					**create_pipes(size_t cmd_cnt);
int					try_command(char *cmd, char **arg, char **env);
char				*process_heredoc(char *limiter);
char				*replace_env_vars(char *str, char **env);
char				*translate_like_bash(char **args);
int					is_command(char *str);
int					is_directory(char *str);
void				ft_free_str3(char ***strs);
void				ft_free_arrs(int **arr);
t_token				*head_token(t_token *token);

#endif