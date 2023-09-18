/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 18:27:37 by gade-oli          #+#    #+#             */
/*   Updated: 2023/09/18 22:28:22 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex_bonus.h"

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
 * executes the first command from the pipe and
 * returns the reading end where the next command has to read
 */
int	first_child(char **argv, char **envp)
{
	char	*infile;
	char	*cmd;
	int		pid;
	int		infile_fd;

	(void)envp;
	infile = argv[0];
	cmd = argv[1]; //TODO: get_full_command
	pid = fork();
	if (pid == 0)
	{
		if (!cmd)
			return (ft_error("error: first command not available"));
		infile_fd = open(infile, O_RDONLY);
		//TODO: gestionar fd invalido?? o lo hace execve??
		execve(cmd, , envp);
	}
	close(fd[1]);
	return (fd[0]);
}

void	pipex_bonus(int argc, char **argv, char **envp)
{
	int	ncmds;
	int i;
	int	fd_write;
	int	fd[2];

	ncmds = argc - 3;
	first_child(argv, envp);
	while (i < ncmds)
	{
		//TODO: implement middle child: get the read end from previous child from pipe
		//father makes all the pipes and fork		
		i++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	int res;
	int heredoc;

	if (BONUS && argc < 5)
		return (ft_error("error format: ./pipex infile cmd1 cmd2 ... cmdn outfile"));
	heredoc = 0; //TODO: detect here_doc
	if (BONUS && !heredoc)
		res = pipex_bonus(argc, argv, envp);
	return (res);
}
