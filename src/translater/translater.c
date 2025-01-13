/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translater.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:22:27 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/13 17:28:54 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	process_edges(char **args, size_t *i, size_t arg_len, char **out)
{
	if (ft_isequal(args[*i], "here_doc"))
	{
		*out = xsafe(end, (t_fvoid)ft_strjoin_safe, 4, *out, args[*i + 2], 1,
				0);
		*out = xsafe(end, (t_fvoid)ft_strjoin_safe, 4, *out, "<< ", 1, 0);
		*out = xsafe(end, (t_fvoid)ft_strjoin_safe, 4, *out, args[*i + 1], 1,
				0);
		(*i) += 3;
		if (arg_len != 4)
		{
			*out = xsafe(end, (t_fvoid)ft_strjoin_safe, 4, *out, "| ", 1, 0);
		}
	}
	else
	{
		*out = xsafe(end, (t_fvoid)ft_strjoin_safe, 4, *out, "< ", 1, 0);
		*out = xsafe(end, (t_fvoid)ft_strjoin_safe, 4, *out, args[0], 1, 0);
		*out = xsafe(end, (t_fvoid)ft_strjoin_safe, 4, *out, " ", 1, 0);
		(*i)++;
	}
}

char	*translate_like_bash(char **args)
{
	size_t	i;
	char	*out;
	size_t	len;

	len = ft_strslen((const char **)args);
	out = NULL;
	i = 0;
	process_edges(args, &i, len, &out);
	while (i < len)
	{
		if (i == len - 2)
		{
			out = xsafe(end, (t_fvoid)ft_strjoin_safe, 4, out, args[len - 2], 1,
					0);
			out = xsafe(end, (t_fvoid)ft_strjoin_safe, 4, out, "> ", 1, 0);
			out = xsafe(end, (t_fvoid)ft_strjoin_safe, 4, out, args[len - 1], 1,
					0);
			break ;
		}
		out = xsafe(end, (t_fvoid)ft_strjoin_safe, 4, out, args[i], 1, 0);
		out = xsafe(end, (t_fvoid)ft_strjoin_safe, 4, out, "|", 1, 0);
		i++;
	}
	return (out);
}
