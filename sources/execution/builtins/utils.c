/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbouagou <bbouagou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 11:05:32 by bbouagou          #+#    #+#             */
/*   Updated: 2023/06/24 01:39:17 by bbouagou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	get_env_nb(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

void	ft_free_pointers(char *ptr1, char *ptr2, char *ptr3)
{
	if (ptr1)
		free (ptr1);
	if (ptr2)
		free (ptr2);
	if (ptr3)
		free (ptr3);
}

char	*setup_variable(char **args, int i, int flag)
{
	char	*string;
	char	*tmp;

	if (!flag)
		string = ft_substr(args[i], 0,
				(size_t)(ft_strchr(args[i], '=') - args[i]));
	else
	{
		tmp = ft_substr(args[i], 0,
				(size_t)(ft_strchr(args[i], '=') - args[i]));
		string = ft_getenv(tmp, g_vars->env);
		ft_free_pointers(tmp, NULL, NULL);
	}
	return (string);
}

void	print_variable(char *value)
{
	int	i;

	i = 0;
	ft_putstr_fd("=", STDOUT_FILENO);
	ft_putstr_fd("\"", STDOUT_FILENO);
	while (value && value[i])
	{
		if (value[i] == '$' || value[i] == '\"')
			ft_putstr_fd("\\", STDOUT_FILENO);
		ft_putchar_fd(value[i], STDOUT_FILENO);
		i++;
	}
	ft_putstr_fd("\"", STDOUT_FILENO);
}

void	edit_variable(char **args, char *string, int i)
{
	ft_setenv(string, ft_substr(args[i],
			(size_t)(ft_strchr(args[i], '=') - args[i]) + 1,
			ft_strlen(args[i])), g_vars->env);
}
