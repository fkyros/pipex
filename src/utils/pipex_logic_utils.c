/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_logic_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 15:13:50 by gade-oli          #+#    #+#             */
/*   Updated: 2023/11/24 11:34:31 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/pipex.h"

int	get_infile_fd(t_pipex pipex)
{
	int	infile_fd;

	if (pipex.here_doc)
		infile_fd = here_doc(pipex);
	else
	{
		infile_fd = open(pipex.infile, O_RDONLY);
		if (infile_fd == -1)
			exit(ft_file_error(pipex.infile));
	}
	return (infile_fd);
}

int	get_oflags_last_child(t_pipex pipex)
{
	int	oflag;

	if (!pipex.here_doc)
		oflag = O_CREAT | O_TRUNC | O_WRONLY;
	else
		oflag = O_CREAT | O_APPEND | O_WRONLY;
	return (oflag);
}

int	get_status_code(t_pipex pipex)
{
	char	**last_cmd;
	int		status;
	int		outfd;
	int		infd;

	if (!pipex.here_doc)
	{
		infd = open(pipex.infile, O_RDONLY);
		if (infd == -1)
			return (FILE_FAIL);
		close(infd);
	}
	status = 0;
	last_cmd = get_full_command(pipex, pipex.ncmds - 1);
	if (access(last_cmd[0], F_OK) == -1)
		status = COMMAND_FAIL;
	else
		free_command(last_cmd);
	outfd = open(pipex.outfile, pipex.outfile_oflag, 0666);
	if (outfd == -1)
		status = FILE_FAIL;
	close(outfd);
	return (status);
}
