/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 18:27:37 by gade-oli          #+#    #+#             */
/*   Updated: 2023/09/29 21:43:16 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

//TODO: quien gestiona si el fd es -1?? execve o yo??

/**
 * executes the first command from the pipe and
 * returns the reading end where the next command has to read from
 */
int	first_child(t_pipex pipex, int fd[2])
{
	char	**cmd;
	pid_t	pid;
	int		infile_fd;

	pid = fork_with_error_check();
		cmd = get_full_command(pipex, 0);
	if (pid == 0)
	{
		if (!cmd)
			return (ft_command_error(cmd[0]));
		infile_fd = open(pipex.infile, O_RDONLY);
		if (infile_fd == -1)
			return ft_file_error(pipex.infile);
		close(fd[READ_END]);
		dup2(infile_fd, STDIN_FILENO);
		close(infile_fd);
		dup2(fd[WRITE_END], STDOUT_FILENO);
		close(fd[WRITE_END]);
		execve(cmd[0], cmd, pipex.envp);
		perror("first child: fail executing command");
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
		if (!cmd)
		{
			ft_command_error(cmd[0]);
			return ;
		}
		dup2(pipex.fd_to_read_from, STDIN_FILENO);
		close(pipex.fd_to_read_from);
		dup2(fd_to_write_in, STDOUT_FILENO);
		close(fd_to_write_in);
		execve(cmd[0], cmd, pipex.envp);
		perror("middle child: fail executing command");
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

	pid = fork_with_error_check();
	if (pid == 0)
	{
		cmd = get_full_command(pipex, pipex.ncmds - 1);
		if (!cmd)
			return (ft_command_error(cmd[0]));
		outfile_fd = open(pipex.outfile, O_CREAT | O_TRUNC | O_WRONLY, 0664);
		if (outfile_fd == -1)
			return ft_file_error(pipex.infile);
		dup2(pipex.fd_to_read_from, STDIN_FILENO);
		close(pipex.fd_to_read_from);
		dup2(outfile_fd, STDOUT_FILENO);
		close(outfile_fd);
		execve(cmd[0], cmd, pipex.envp);
		perror("last child: fail executing command");
		exit(FAIL);
	}
	close(pipex.fd_to_read_from);
	return (pid);
}

int	pipex_logic(t_pipex pipex)
{
	int		res;
	int 	i;
	int		fd[2];
	pid_t	pid;
	int		status;

	pipe_with_error_check(fd);
	pipex.fd_to_read_from = first_child(pipex, fd);
	i = 1;
	while (i < pipex.ncmds - 1)
	{
		pipe_with_error_check(fd);
		pid = fork_with_error_check();
		if (pid == 0)
			middle_child(pipex, i, fd[WRITE_END]);
		pipex.fd_to_read_from = fd[READ_END];
		close(fd[WRITE_END]);
		//TODO: gestionar fds
		i++;
	}
	//TODO: parent wait and frees
	pid = last_child(pipex);
	waitpid(pid, &status, 0);
	res = 0;
	if (WIFEXITED(pid))
		res = WEXITSTATUS(status);
	return (res);
}