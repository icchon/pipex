/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 15:48:19 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/04 18:35:06 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*replace_env_vars(char *str)
{
	size_t	i;
	char	*key;
	char	*value;
	char	*out;
	char	*normal;
	size_t	len;

	out = NULL;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			len = ft_calc_next_str(&str[i], "\t\n\v\f\r  $");
			key = ft_substr(&str[i], 0, len);
			value = ft_get_env(key);
			out = strjoin_safe(out, value, 1, 0);
		}
		else
		{
			len = ft_calc_next_chr(&str[i], '$');
			normal = ft_substr(&str[i], 0, len);
			out = strjoin_safe(out, normal, 1, 1);
		}
		i += len;
	}
	return (out);
}

int	is_directory(char *str)
{
	if (ft_strchr(str, '/'))
		return (1);
	return (0);
}

int	is_command(char *str)
{
	return (!is_directory(str));
}

int	try_command(char *cmd, char **arg)
{
	char	*path;

	path = ft_get_absolute_path(cmd);
	if (!is_directory(path) || !path)
		return (0);
	if (access(path, X_OK) != 0)
	{
		free(path);
		return (0);
	}
	execve(path, arg, __environ);
	return (0);
}

void	ft_free_arrs(int **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	ft_free_str3(char ***strs)
{
	int	i;

	i = 0;
	while (strs[i])
	{
		ft_free_strs(strs[i]);
		i++;
	}
	free(strs);
	return ;
}

int	**create_pipes(size_t cmd_cnt)
{
	int		**pipes;
	size_t	i;

	pipes = (int **)malloc(sizeof(int *) * (cmd_cnt + 1));
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < cmd_cnt)
	{
		pipes[i] = (int *)malloc(sizeof(int) * 2);
		if (!pipes[i++])
			return (ft_free_arrs(pipes), NULL);
	}
	pipes[cmd_cnt] = NULL;
	return (pipes);
}

int	std_pipe(t_vars *data)
{
	extern char	**__environ;
	int			**pipes;
	pid_t		pid;
	char		buff[BUFFER_SIZE];
	int			status;
	int			file_fds[2];
	int			res;
	char		*msg;
	size_t		i;
	size_t		cmd_cnt;

	cmd_cnt = ft_strslen((const char **)data->cmds);
	pipes = create_pipes(cmd_cnt);
	file_fds[INPUT] = open(data->infile, O_RDONLY);
	if (file_fds[INPUT] == -1)
	{
		msg = ft_strjoin("bash: ", data->infile);
		return (perror(msg), free(msg), 0);
	}
	dup2(file_fds[INPUT], STDIN_FILENO);
	close(file_fds[INPUT]);
	file_fds[OUTPUT] = open(data->outfile, O_WRONLY);
	if (file_fds[OUTPUT] == -1)
	{
		if (ft_path_exist(data->outfile))
		{
			msg = ft_strjoin("bash: ", data->outfile);
			return (perror(msg), free(msg), 0);
		}
		close(ft_create_file(data->outfile));
		file_fds[OUTPUT] = open(data->outfile, O_WRONLY);
		if (file_fds[OUTPUT] == -1)
		{
			perror(NULL);
			return (0);
		}
	}
	close(file_fds[OUTPUT]);
	i = 0;
	while (i < cmd_cnt)
	{
		pipe(pipes[i]);
		pid = fork();
		if (pid == CHILD_PID)
		{
			close(pipes[i][READ]);
			dup2(pipes[i][WRITE], STDOUT_FILENO);
			close(pipes[i][WRITE]);
			res = try_command(data->cmds[i], data->cmd_args[i]);
			if (!res)
			{
				if (is_command(data->cmds[i]))
					ft_dprintf(2, "%s: command not found\n", data->cmds[i]);
				else
					ft_dprintf(2, "bash: %s: No such file or directory\n",
						data->cmds[i]);
			}
			exit(EXIT_FAILURE);
		}
		wait(&status);
		if (status != EXIT_SUCCESS)
			exit(EXIT_FAILURE);
		close(pipes[i][WRITE]);
		dup2(pipes[i][READ], STDIN_FILENO);
		close(pipes[i][READ]);
		i++;
	}
	ft_bzero(buff, BUFFER_SIZE);
	read(STDIN_FILENO, buff, BUFFER_SIZE);
	if (data->is_heredoc)
		file_fds[OUTPUT] = open(data->outfile, O_WRONLY | O_APPEND);
	else
		file_fds[OUTPUT] = open(data->outfile, O_WRONLY | O_TRUNC);
	ft_dprintf(file_fds[OUTPUT], buff);
	close(file_fds[OUTPUT]);
	return (1);
}

char	*process_heredoc(char *limiter)
{
	char	*input_file;
	char	buff[BUFFER_SIZE];
	char	tmp[BUFFER_SIZE];
	int		fd;

	ft_bzero(buff, BUFFER_SIZE);
	input_file = ft_create_random_file(".tmp");
	while (1)
	{
		ft_printf("> ");
		ft_bzero(tmp, BUFFER_SIZE);
		read(STDIN_FILENO, tmp, BUFFER_SIZE);
		tmp[ft_strlen(tmp) - 1] = '\0';
		if (ft_isequal(limiter, tmp))
			break ;
		ft_strlcat(buff, tmp, BUFFER_SIZE);
		ft_strlcat(buff, "\n", BUFFER_SIZE);
	}
	fd = open(input_file, O_WRONLY);
	ft_dprintf(fd, buff);
	close(fd);
	return (input_file);
}

char	**expand_args(char **args)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (args[i])
	{
		args[i] = replace_env_vars(args[i]);
		if (!args[i])
		{
			j = 0;
			while (args[i + j])
			{
				args[i + j] = args[i + j + 1];
				j++;
			}
		}
		i++;
	}
	return (args);
}

int	main(int argc, char *argv[])
{
	extern char	**__environ;
	char		**tmp;
	size_t		arg_idx;
	size_t		cmd_idx;
	t_vars		data;
	char		**args;

	// char		*input;
	// char		*output;
	if (argc < 5)
		return (0);
	args = expand_args(&argv[1]);
	arg_idx = 0;
	data.infile = NULL;
	data.is_heredoc = 0;
	if (ft_isequal(args[arg_idx], "here_doc"))
	{
		arg_idx++;
		data.infile = process_heredoc(args[arg_idx]);
		arg_idx++;
		data.is_heredoc = 1;
	}
	else
	{
		data.infile = ft_strdup(args[arg_idx]);
		arg_idx++;
	}
	data.cmd_cnt = (ft_strslen((const char **)args) - arg_idx) - 1;
	data.cmds = (char **)malloc(sizeof(char *) * (data.cmd_cnt + 1));
	data.cmds[data.cmd_cnt] = NULL;
	data.cmd_args = (char ***)malloc(sizeof(char **) * (data.cmd_cnt + 1));
	data.cmd_args[data.cmd_cnt] = NULL;
	cmd_idx = 0;
	while (arg_idx < data.cmd_cnt + 1)
	{
		tmp = ft_split(args[arg_idx], ' ');
		data.cmds[cmd_idx] = tmp[0];
		data.cmd_args[cmd_idx] = tmp;
		cmd_idx++;
		arg_idx++;
	}
	data.outfile = args[arg_idx];
	std_pipe(&data);
	if (data.is_heredoc)
		unlink(data.infile);
	return (0);
}
