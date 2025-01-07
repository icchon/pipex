/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translater.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:22:27 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/05 13:22:36 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*translate_like_bash(char **args)
{
	size_t	i;
	char	*out;
	size_t	arg_len;

	arg_len = ft_strslen((const char **)args);
	out = NULL;
	i = 0;
	if (ft_isequal(args[i], "here_doc"))
	{
		out = ft_strjoin_safe(out, args[i + 2], 1, 0);
		out = ft_strjoin_safe(out, "<< ", 1, 0);
		out = ft_strjoin_safe(out, args[i + 1], 1, 0);
		i += 3;
		if (arg_len != 4)
		{
			out = ft_strjoin_safe(out, "| ", 1, 0);
		}
	}
	else
	{
		out = ft_strjoin_safe(out, "< ", 1, 0);
		out = ft_strjoin_safe(out, args[0], 1, 0);
		out = ft_strjoin_safe(out, " ", 1, 0);
		i++;
	}
	while (i < arg_len)
	{
		if (i == arg_len - 2)
		{
			out = ft_strjoin_safe(out, args[arg_len - 2], 1, 0);
			out = ft_strjoin_safe(out, "> ", 1, 0);
			out = ft_strjoin_safe(out, args[arg_len - 1], 1, 0);
			break ;
		}
		out = ft_strjoin_safe(out, args[i], 1, 0);
		out = ft_strjoin_safe(out, "| ", 1, 0);
		i++;
	}
	printf("%s\n", out);
	return (out);
}
