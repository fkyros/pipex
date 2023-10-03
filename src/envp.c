/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 21:54:08 by gade-oli          #+#    #+#             */
/*   Updated: 2023/10/03 18:49:05 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

/**
 * given the enviromental variable, returns all the folders the PATH has
 */
char	**get_paths_from_envp(char **envp)
{
	char	*path;
	char	**dir_bins;
	int		i;

	if (!envp)
		return (NULL);
	path = NULL;
	i = 0;
	while (!path && envp[i])
	{
		if (ft_strnstr(envp[i], "PATH=", 5))
			path = envp[i];
		i++;
	}
	if (!path)
		return (NULL);
	dir_bins = ft_split(path, ':');
	return (dir_bins);
}

/**
 * returns the full path of a command, given the enviroment variables
 */
char	*get_commands_full_path(char *command, char **envp)
{
	char	*res;
	char	**dir_bins;
	int		permitted;
	int		i;
	char	*tmp;

	dir_bins = get_paths_from_envp(envp);
	if (!dir_bins)
		return (command);
	permitted = -1;
	i = 0;
	while (permitted == -1 && dir_bins[i])
	{
		tmp = ft_strjoin(dir_bins[i], "/");
		res = ft_strjoin(tmp, command);
		free(tmp);
		permitted = access(res, X_OK);
		if (permitted == -1)
			free(res);
		i++;
	}
	free_command(dir_bins);
	if (permitted == -1)
		return (command);
	return (res);
}

/**
 * returns a char matrix of the given command
 * with the executable path
 */
char	**get_full_command(t_pipex pipex, int ncmd)
{
	char	*raw_command;
	char	**res;
	char	*full_cmd_path;
	char	*tmp;
	int		desp;

	if (!pipex.here_doc)
		desp = 2;
	else
		desp = 3;
	raw_command = pipex.argv[ncmd + desp];
	if (raw_command ==  NULL)
		return (NULL);
	res = ft_split(raw_command, ' ');
	if (!res)
		return (NULL);
	if (!ft_strncmp("/", raw_command, 1)
		|| !ft_strncmp("./", raw_command, 2)
		|| !ft_strncmp("../", raw_command,3)
		|| !ft_strncmp("~/", raw_command,2))
		return (res);
	full_cmd_path = get_commands_full_path(res[0], pipex.envp);
	if (!full_cmd_path)
		return (res);
	if (ft_strcmp(full_cmd_path, raw_command) == 0
		&& access(raw_command, F_OK) == 0)
		raw_command = ft_strjoin("./", raw_command);
	tmp = *res;
	*res = full_cmd_path;
	free(tmp);
	return (res);
}
