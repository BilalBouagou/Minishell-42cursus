/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbouagou <bbouagou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 01:47:00 by bbouagou          #+#    #+#             */
/*   Updated: 2023/06/24 11:40:46 by bbouagou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	exec_builtin(t_parser *list)
{
	if (list->args && list->args[0] && !ft_strcmp(list->args[0], "echo"))
		g_vars->exit_status = ft_echo(list->args, list);
	else if (list->args && list->args[0] && !ft_strcmp(list->args[0], "pwd"))
		g_vars->exit_status = ft_pwd(list->args, list);
	else if (list->args && list->args[0] && !ft_strcmp(list->args[0], "cd"))
		g_vars->exit_status = ft_cd(list->args, g_vars->env, list);
	else if (list->args && list->args[0] && !ft_strcmp(list->args[0], "export"))
		g_vars->exit_status = ft_export(list->args, list);
	else if (list->args && list->args[0] && !ft_strcmp(list->args[0], "unset"))
		g_vars->exit_status = ft_unset(list->args, list);
	else if (list->args && list->args[0] && !ft_strcmp(list->args[0], "env"))
		g_vars->exit_status = ft_env(g_vars->env, list);
	else if (list->args && list->args[0] && !ft_strcmp(list->args[0], "exit"))
		g_vars->exit_status = ft_exit(list->args, list);
	else
		return (0);
	return (1);
}

static int	is_a_builtin(t_parser *list)
{
	if (list->args && list->args[0] && (!ft_strcmp(list->args[0], "echo")
			|| !ft_strcmp(list->args[0], "pwd")
			|| !ft_strcmp(list->args[0], "cd")
			|| !ft_strcmp(list->args[0], "export")
			|| !ft_strcmp(list->args[0], "unset")
			|| !ft_strcmp(list->args[0], "env")
			|| !ft_strcmp(list->args[0], "exit")))
		return (1);
	else
		return (0);
}

static char	*search_for_cmd(char *cmd, char **path)
{
	int		i;
	char	*tmp;

	i = -1;
	tmp = NULL;
	if (cmd && cmd[0] && is_dir(cmd) == 1)
		exit(126);
	else if (cmd && cmd[0] && access(cmd, X_OK) == 0)
		return (cmd);
	else if (cmd && cmd[0] && !access(cmd, F_OK) && access(cmd, X_OK))
		ft_print_error(cmd, ": Permission denied", 126);
	else if ((cmd && cmd[0] && ft_strchr(cmd, '/')
			&& access(cmd, F_OK) == -1) || !path)
		ft_print_error(cmd, ": No such file or directory", 127);
	while (path && cmd && cmd[0] && path[++i])
	{
		tmp = string_join(ft_strdup(path[i]), ft_strdup("/"));
		tmp = string_join(tmp, ft_strdup(cmd));
		if (access(tmp, X_OK) == 0)
			return (tmp);
		free (tmp);
	}
	return (NULL);
}

static void	exec_cmd(t_parser *list, t_exec *es)
{
	char	**path;
	char	*cmd;

	cmd = NULL;
	if (exec_builtin(list) == 0)
	{
		path = ft_split(ft_getenv("PATH", g_vars->env), ':');
		if (list->args)
			cmd = search_for_cmd(list->args[0], path);
		clean(path);
		if (cmd)
			if (execve(cmd, list->args, g_vars->env))
				exit(ft_perror("execve : "));
		if (list->args || (list->args && !list->args[0]))
		{
			multi_purpose_func(NULL, list->args[0], 1);
			exit(127);
		}
		exit(0);
	}
	else if (list->prev->type == __PIPE)
		exit(g_vars->exit_status);
	multi_purpose_func(es, NULL, 0);
}

void	executor(t_parser *list)
{
	t_exec				*es;

	es = init_struct(list);
	while (list)
	{
		list->heredoc = mount_heredoc(list->file);
		heredoc_handle(list->heredoc, es->heredoc);
		if (!is_a_builtin(list)
			|| (is_a_builtin(list) && list->prev->type == __PIPE))
		{
			pipe(es->pipefd);
			es->nb_commands++;
			es->pid = fork();
			if (es->pid == 0)
			{
				pipes_handle(list, es->old_fd, es->pipefd, es->heredoc);
				exec_cmd(list, es);
			}
			close_fds(es, list);
		}
		else
			exec_cmd(list, es);
		list = list->next;
	}
	get_exit_status(es->pid, es);
}
