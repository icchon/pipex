/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:39:35 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/09 15:55:01 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char **grobal_env(int get_or_set, char **env){
	static char **g_env;
	if(get_or_set == GET){
		return g_env;
	}
	else{
		g_env = env;
	}
	return g_env;
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

char *replace_env_vars_quate(char *str, char **env){
	size_t i;
	i = 0;
	char *out = NULL;
	char *part;
	size_t len = ft_calc_next_chr(&str[i], '\'');
	if(str[i] == '\''){
		len = ft_calc_next_chr(&str[i+1], '\'');
		part = ft_substr(&str[i], 0, len+2);
		out = ft_strjoin_safe(out, part, 1, 1);
		i+=len +2;
	}
	while(len > 0){
		part = ft_substr(&str[i], 0, len);
		part = replace_env_vars(part, env);
		out = ft_strjoin_safe(out, part, 1, 1);
		i+= len;
		if(str[i] == '\''){
			len = ft_calc_next_chr(&str[i+1], '\'');
			part = ft_substr(&str[i], 0, len+2);
			out = ft_strjoin_safe(out, part, 1, 1);
			i+=len +2;
		}
		len = ft_calc_next_chr(&str[i], '\'');
	}
	return out;
	
}

void expander(t_astnode *node)
{
	if(!node){
		return;
	}
	t_redirect *redirect;
	t_arg *arg;
	t_cmd *cmd;
	redirect = node->redirects;
	arg = node->args;
	cmd = node->cmd;
	if(cmd)
		node->cmd->data = replace_env_vars_quate(node->cmd->data, grobal_env(GET, NULL));
	while(redirect){
		redirect->data = replace_env_vars_quate(redirect->data, grobal_env(GET, NULL));
		redirect = redirect->next;
	}
	while(arg){
		arg->data = replace_env_vars_quate(arg->data, grobal_env(GET, NULL));
		arg = arg->next;
	}
	expander(node->left);
	expander(node->right);
	return ;
}
