/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:41:39 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/24 12:44:49 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	handle_io(t_token *redirect)
{
	int	fd;

	while (redirect)
	{
		if (redirect->type == TK_INPUT_FILE)
		{
			fd = open(redirect->data, O_RDONLY);
			dup2(fd, STDIN_FILENO);
		}
		else if (redirect->type == TK_OUTPUT_FILE)
		{
			fd = open(redirect->data, O_WRONLY | O_TRUNC | O_CREAT,
					S_IRGRP | S_IROTH | S_IWUSR | S_IRUSR);
			dup2(fd, STDOUT_FILENO);
		}
		else if (redirect->type == TK_OUTPUT_FILE_APPEND)
		{
			fd = open(redirect->data, O_WRONLY | O_APPEND | O_CREAT,
					S_IRGRP | S_IROTH | S_IWUSR | S_IRUSR);
			dup2(fd, STDOUT_FILENO);
		}
		redirect = redirect->next;
	}
}

static int	try_command(char *cmd, char **arg, char **env)
{
	char	*path;

	path = ft_get_absolute_path(cmd, env);
	if (!is_directory(path) || !path)
	{
		free(path);
		return (0);
	}
	if (access(path, X_OK) != 0)
	{
		free(path);
		return (0);
	}
	execve(path, arg, env);
	return (0);
}

static void	child_process(int pipes[2], t_token *redirect, t_astnode *node)
{
	int	res;

	close(pipes[READ]);
	dup2(pipes[WRITE], STDOUT_FILENO);
	handle_io(redirect);
	res = try_command(node->cmd->data, node->arg_strs, grobal_env(GET, NULL));
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

int	exec_command(t_astnode *node)
{
	int			pipes[2];
	int			status;
	t_redirect	*redirect;
	pid_t		pid;

	redirect = node->redirects;
	pipe(pipes);
	pid = fork();
	if (pid == CHILD_PID)
	{
		child_process(pipes, redirect, node);
	}
	wait(&status);
	close(pipes[WRITE]);
	dup2(pipes[READ], STDIN_FILENO);
	close(pipes[READ]);
	if (status != EXIT_SUCCESS)
		return (0);
	return (1);
}
