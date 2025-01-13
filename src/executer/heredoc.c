/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:23:02 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/13 22:01:00 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*process_heredoc(char *limiter)
{
	char	*input_file;
	char	buff[BUFFER_SIZE];
	char	tmp[BUFFER_SIZE];
	int		fd;

	ft_bzero(buff, BUFFER_SIZE);
	input_file = xsafe(end, (t_fvoid)ft_create_random_file, 1, ".tmp");
	grobal_tmpfile(SET, input_file);
	while (1)
	{
		ft_printf("heredoc> ");
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

void	exec_heredoc(t_astnode *node)
{
	t_redirect	*redirect;
	char		*limiter;

	if (!node)
	{
		return ;
	}
	redirect = node->redirects;
	while (redirect)
	{
		if (redirect->type == TK_LIMITER)
		{
			limiter = redirect->data;
			redirect->data = process_heredoc(limiter);
			redirect->data = xsafe(end, (t_fvoid)replace_env_vars, 2,
					redirect->data, grobal_env(GET, NULL));
			redirect->type = TK_INPUT_FILE;
			free(limiter);
		}
		redirect = redirect->next;
	}
	exec_heredoc(node->left);
	exec_heredoc(node->right);
}
