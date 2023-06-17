/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbouagou <bbouagou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 11:45:42 by bbouagou          #+#    #+#             */
/*   Updated: 2023/06/17 10:51:29 by bbouagou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	clean(char **var)
{
	int	i;

	i = -1;
	while (var[++i])
		free(var[i]);
	free(var);
}

char	*ft_getenv(char *name, char *env[])
{
	char	**var;
	char	*value;
	int		i;

	var = NULL;
	value = NULL;
	i = -1;
	while (env && env[++i])
	{
		var = ft_split(env[i], '=');
		if (!strcmp(name, var[0]))
		{
			if (var[1])
				value = ft_strdup(var[1]);
			else
				value = NULL;
			clean(var);
			return (value);
		}
		clean(var);
	}
	return (NULL);
}