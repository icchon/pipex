/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:45:25 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/07 20:37:55 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	try_command(char *cmd, char **arg, char **env)
{
	char	*path;

	path = ft_get_absolute_path(cmd, env);
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
