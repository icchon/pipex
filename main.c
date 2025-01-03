/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 15:48:19 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/03 18:33:16 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	try_command(char *cmd, char **arg)
{
	char	*path;

	path = ft_get_absolute_path(cmd);
	if (!path)
		return (0);
	if (access(path, X_OK) != 0)
	{
		free(path);
		return (0);
	}
	execve(path, arg, __environ);
	return (0);
}

int	create_file(char *filename)
{
	return (open(filename, O_CREAT, S_IRGRP | S_IROTH | S_IWUSR | S_IRUSR));
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

int	main(int argc, char *argv[])
{
	char		*infile;
	char		*outfile;
	extern char	**__environ;
	int			**pipes;
	pid_t		pid;
	char		buff[10000];
	int			status;
	char		**tmp;
	int			file_fds[2];
	int			res;
	char		*msg;
	char		**cmds;
	char		***cmd_args;
	size_t		i;
	size_t		cmd_cnt;

	ft_bzero(buff, 10000);
	if (argc < 5)
		return (0);
	cmd_cnt = argc - 1 - 2;
	pipes = (int **)malloc(sizeof(int *) * (cmd_cnt + 1));
	if (!pipes)
		return (0);
	i = 0;
	while (i < cmd_cnt)
	{
		pipes[i] = (int *)malloc(sizeof(int) * 2);
		if (!pipes[i++])
			return (ft_free_arrs(pipes), 0);
	}
	pipes[cmd_cnt] = NULL;
	cmds = (char **)malloc(sizeof(char *) * (cmd_cnt + 1));
	if (!cmds)
		return (ft_free_arrs(pipes), 0);
	cmds[cmd_cnt] = NULL;
	cmd_args = (char ***)malloc(sizeof(char **) * (cmd_cnt + 1));
	if (!cmd_args)
		return (ft_free_arrs(pipes), 0);
	cmd_args[cmd_cnt] = NULL;
	infile = argv[1];
	i = 0;
	while (i < cmd_cnt)
	{
		tmp = ft_split(argv[i + 2], ' ');
		cmds[i] = tmp[0];
		cmd_args[i] = tmp;
		i++;
	}
	outfile = argv[cmd_cnt + 2];
	file_fds[INPUT] = open(infile, O_RDONLY);
	if (file_fds[INPUT] == -1)
	{
		msg = ft_strjoin("bash: ", infile);
		return (perror(msg), free(msg), 0);
	}
	dup2(file_fds[INPUT], STDIN_FILENO);
	close(file_fds[INPUT]);
	file_fds[OUTPUT] = open(outfile, O_WRONLY);
	if (file_fds[OUTPUT] <= 0)
	{
		msg = ft_strjoin("bash: ", outfile);
		return (perror(msg), 0);
	}
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
			res = try_command(cmds[i], cmd_args[i]);
			if (!res)
				ft_dprintf(2, "%s: command not found\n", cmds[i]);
			exit(1);
		}
		wait(&status);
		close(pipes[i][WRITE]);
		dup2(pipes[i][READ], STDIN_FILENO);
		close(pipes[i][READ]);
		i++;
	}
	ft_bzero(buff, 10000);
	read(STDIN_FILENO, buff, 10000);
	if (!ft_path_exist(outfile))
		create_file(outfile);
	res = write(file_fds[OUTPUT], buff, 10000);
	if (res == -1)
		perror(NULL);
	close(file_fds[OUTPUT]);
	ft_free_str3(cmd_args);
	ft_free_arrs(pipes);
	return (0);
}
