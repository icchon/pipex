/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xsafe.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisobe <kaisobe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 22:39:14 by kaisobe           #+#    #+#             */
/*   Updated: 2025/01/13 22:39:27 by kaisobe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	*xsafe(void (*g)(void), t_fvoid f, int argn, ...)
{
	void	*res;
	va_list	ap;
	size_t	i;
	void	*args[4];

	i = 0;
	va_start(ap, argn);
	while (i < (size_t)argn)
		args[i++] = va_arg(ap, void *);
	va_end(ap);
	if (argn == 1)
		res = f(args[0]);
	else if (argn == 2)
		res = f(args[0], args[1]);
	else if (argn == 3)
		res = f(args[0], args[1], args[2]);
	else
		res = f(args[0], args[1], args[2], args[3]);
	if (!res)
	{
		g();
		exit(EXIT_FAILURE);
	}
	return (res);
}
