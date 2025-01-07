/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:39:35 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/05 13:45:04 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**expand_args(char **args, char **env)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (args[i])
	{
		args[i] = replace_env_vars(args[i], env);
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

char	*replace_env_vars(char *str, char **env)
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
			value = ft_get_env(key, env);
			out = ft_strjoin_safe(out, value, 1, 0);
		}
		else
		{
			len = ft_calc_next_chr(&str[i], '$');
			normal = ft_substr(&str[i], 0, len);
			out = ft_strjoin_safe(out, normal, 1, 1);
		}
		i += len;
	}
	return (out);
}
