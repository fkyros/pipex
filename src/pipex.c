/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 18:27:37 by gade-oli          #+#    #+#             */
/*   Updated: 2023/11/13 15:00:19 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

int	first_child(t_pipex pipex, int fd[2])
{
	char	**cmd;
	pid_t	pid;
	int		infile_fd;

	pid = fork_with_error_check();
	if (pid == 0)
	{
		cmd = get_full_command(pipex, 0);
		infile_fd = get_infile_fd(pipex);
		close(fd[READ_END]);
		dup2(infile_fd, STDIN_FILENO);
		close(infile_fd);
		dup2(fd[WRITE_END], STDOUT_FILENO);
		close(fd[WRITE_END]);
		execve(cmd[0], cmd, pipex.envp);
		ft_command_error(cmd[0]);
		exit(FAIL);
	}
	close(fd[WRITE_END]);
	return (fd[READ_END]);
}

void	middle_child(t_pipex pipex, int ncmd, int fd_to_write_in)
{
	char	**cmd;
	pid_t	pid;

	pid = fork_with_error_check();
	if (pid == 0)
	{
		cmd = get_full_command(pipex, ncmd);
		dup2(pipex.fd_to_read_from, STDIN_FILENO);
		close(pipex.fd_to_read_from);
		dup2(fd_to_write_in, STDOUT_FILENO);
		close(fd_to_write_in);
		execve(cmd[0], cmd, pipex.envp);
		ft_command_error(cmd[0]);
		exit(FAIL);
	}
	close(fd_to_write_in);
	close(pipex.fd_to_read_from);
}

int	last_child(t_pipex pipex)
{
	char	**cmd;
	pid_t	pid;
	int		outfile_fd;

	pipex.outfile_oflag = get_oflags_last_child(pipex);
	pid = fork_with_error_check();
	if (pid == 0)
	{
		cmd = get_full_command(pipex, pipex.ncmds - 1);
		outfile_fd = open(pipex.outfile, pipex.outfile_oflag, 0664);
		if (outfile_fd == -1)
			exit(ft_file_error(pipex.outfile));
		dup2(pipex.fd_to_read_from, STDIN_FILENO);
		close(pipex.fd_to_read_from);
		dup2(outfile_fd, STDOUT_FILENO);
		close(outfile_fd);
		execve(cmd[0], cmd, pipex.envp);
		ft_command_error(cmd[0]);
		exit(FAIL);
	}
	close(pipex.fd_to_read_from);
	return (pid);
}

int	pipex_logic(t_pipex pipex)
{
	int		i;
	int		fd[2];
	pid_t	pid;

	pipe_with_error_check(fd);
	pipex.fd_to_read_from = first_child(pipex, fd);
	if (pipex.here_doc)
		waitpid(ANY_CHILD, NULL, 0);
	i = 1;
	while (i < pipex.ncmds - 1)
	{
		pipe_with_error_check(fd);
		middle_child(pipex, i, fd[WRITE_END]);
		pipex.fd_to_read_from = fd[READ_END];
		i++;
	}
	i++;
	pid = last_child(pipex);
	while (i-- > 0)
		waitpid(ANY_CHILD, NULL, 0);
	//return (get_status_code(pipex));
	return (WEXITSTATUS(pid));
}
