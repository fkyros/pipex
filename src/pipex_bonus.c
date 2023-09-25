/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 18:27:37 by gade-oli          #+#    #+#             */
/*   Updated: 2023/09/25 13:44:42 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex_bonus.h"

//TODO: quitar "bonus" del nombre?
//TODO: error control

/**
 * prints in stderr the message given and returns the failure code
 */
int	ft_error(char *msg)
{
	ft_putstr_fd(msg, 2);
	return (FAIL);
}

/**
 * returns a char matrix of the given command
 * with the executable path
 */
char	**get_full_command(t_pipex pipex, int ncmd)
{
	char	*raw_cmd;
	int		desp;
	char	**res;

	if (!pipex.heredoc)
		desp = 2;
	else
		desp = 3;
	if (!raw_command)
		return (NULL);
	res = ft_spit(raw_command);
	if (!res)
		return (NULL);
	if (!ft_strncmp("/", 1)
		|| !ft_strncmp("./", 2)
		|| !ft_strncmp("../", 3)
		|| !ft_strncmp("~/", 2))
		return (res);
	//TODO: get the commands path with envp varible
}

/**
 * executes the first command from the pipe and
 * returns the reading end where the next command has to read from
 */
int	first_child(t_pipex pipex, int fd[2])
{
	char	**cmd;
	pid_t	pid;
	int		infile_fd;

	cmd = get_full_command(pipex, 0);
	pid = fork_with_error_checl();
	if (pid == 0)
	{
		if (!cmd)
			return (ft_error("error: first command not available"));
		infile_fd = open(pipex.infile, O_RDONLY);
		//TODO: gestionar fd invalido?? o lo hace execve??
		execve(cmd[0], cmd, pipex.envp);
	}
	close(fd[1]);
	return (fd[0]);
}

int	pipex_bonus(t_pipex pipex)
{
	int		res;
	int 	i;
	int		fd_write;
	int		fd[2];
	pid_t	pid;

	pipe_with_error_check(fd);
	pipex.fd_to_read_from = first_child(pipex, fd);
	i = 1;
	while (i < pipex.ncmds - 1)
	{
		//TODO: implement middle child: get the read end from previous child from pipe
		//father makes all the pipes and fork
		pipe_with_error_check(fd);
		pid = fork_with_error_check();
		if (pid == 0)
			middle_child(pipex); //TODO
		pipex.fd_to_read_from = fd[READ_END];
		//TODO: gestionar fds
		i++;
	}
	//TODO: parent wait and frees
	pid = last_child(pipex);
	res = 0;
	if (WIFEXITED(pid))
		res = WEXITSTATUS(pid);
	return (res);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int res;

	if (!BONUS && argc != 5)
		return (ft_error("error format: ./pipex infile cmd1 cmd2 outfile"));
	if (BONUS && ft_strcmp(argv[1], "here_doc") && argc < 5)
		return (ft_error("error format: ./pipex infile cmd1 cmd2 ... cmdn outfile"));
	pipex.envp = envp;
	pipex.argv = argv;
	pipex.here_doc = 0;
	if (argc >= 6 && !ft_strcmp(argv[1], "here_doc"))
		pipex.here_doc = 1;
	if (BONUS && !pipex.here_doc)
	{
		pipex.infile = argv[1];
		pipex.outfile = argv[argc - 1];
		pipex.ncmds = argc - 3;
		res = pipex_bonus(pipex);
	}
	//TODO: implement here_doc
	//TODO: implement (re-use) no bonus -> 2 commands only
	return (res);
}
