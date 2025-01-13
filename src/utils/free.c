/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:44:16 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/13 10:54:38 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
