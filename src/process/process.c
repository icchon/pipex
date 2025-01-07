/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:41:39 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/07 20:41:07 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	std_pipe(t_vars *data, char **env)
{
	int		**pipes;
	pid_t	pid;
	char	buff[BUFFER_SIZE];
	int		status;
	int		file_fds[2];
	int		res;
	char	*msg;
	size_t	i;
	size_t	cmd_cnt;

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
			res = try_command(data->cmds[i], data->cmd_args[i], env);
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
