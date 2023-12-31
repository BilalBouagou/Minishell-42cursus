/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   params_expander_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbouagou <bbouagou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 12:42:37 by bbouagou          #+#    #+#             */
/*   Updated: 2023/06/21 13:31:48 by bbouagou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_valid(char c)
{
	return (ft_isalpha(c) || ft_isdigit(c) || c == '_');
}

int	skip_quotes(char *string, int i)
{
	if (string[i] == '\'')
	{
		i++;
		while (string[i] && string[i] != '\'')
			i++;
		if (string[i] == '\'')
			return (i + 1);
	}
	return (i);
}

char	quote_type(char quote)
{
	if (quote == '\'')
		return ('\'');
	else if (quote == '"')
		return ('\"');
	else
		return (quote);
}
