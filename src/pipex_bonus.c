/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 18:27:37 by gade-oli          #+#    #+#             */
/*   Updated: 2023/09/29 21:09:03 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex_bonus.h"

//TODO: quitar "bonus" del nombre?

void    fre(void)
{
    system("leaks -q pipex");
}

/**
 * prints in stderr the message given and returns the failure code
 */
int	ft_error(char *msg)
{
	ft_putstr_fd(msg, 2);
	return (FAIL);
}

void    free_command(char **matrix)
{
    int i;

    if (!matrix)
        return ;
    i = 0;
    while (matrix[i])
    {
        free(matrix[i]);
        i++;
    }
    free(matrix);
}

/**
 * given the enviromental variable, returns all the folders the PATH has
 */
char    **get_paths_from_envp(char **envp)
{
    char    *path;
    char    **dir_bins;
    int     i;

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
char    *get_commands_full_path(char *command, char **envp)
{
    char    *res;
    char    **dir_bins;
    int     permitted;
    int     i;
    char    *tmp;

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
	if (!raw_command)
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
			return (ft_error("error: first command not available")); //TODO: print error msg with param
		infile_fd = open(pipex.infile, O_RDONLY);
		//TODO: gestionar fd invalido?? o lo hace execve??
		if (infile_fd == -1)
			printf("infile fd no valido, no me encargo lol\n"); //TODO: quitar
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
		cmd = get_full_command(pipex, ncmd);
	if (pid == 0)
	{
		if (!cmd)
		{
			ft_error("error: command(s) in the middle doesn't exist");
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
		cmd = get_full_command(pipex, pipex.ncmds - 1);
	if (pid == 0)
	{
		if (!cmd)
			return (ft_error("error: last command not available"));
		outfile_fd = open(pipex.outfile, O_CREAT | O_TRUNC | O_WRONLY, 0664);
		//TODO: gestionar fd invalido?? o lo hace execve??
		if (outfile_fd == -1)
			printf("infile fd no valido, no me encargo lol\n"); //TODO: quitar
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

int	pipex_bonus(t_pipex pipex)
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

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int res;

	if (!BONUS && argc != 5)
		return (ft_error("error format: ./pipex infile cmd1 cmd2 outfile\n"));
	if (BONUS && ft_strcmp(argv[1], "here_doc") && argc < 4)
		return (ft_error("error format: ./pipex infile cmd1 cmd2 ... cmdn outfile\n"));
	if (BONUS && !ft_strcmp(argv[1], "here_doc") && argc != 6)
		return (ft_error("error format: ./pipex here_doc LIMITER cmd1 cmd2 outfile\n"));
	pipex.envp = envp;
	pipex.argv = argv;
	pipex.here_doc = 0;
	if (argc >= 6 && !ft_strcmp(argv[1], "here_doc"))
		pipex.here_doc = 1;
	if ((BONUS && !pipex.here_doc) || (!BONUS && argc == 5))
	{
		pipex.infile = argv[1];
		pipex.outfile = argv[argc - 1];
		pipex.ncmds = argc - 3;
		res = pipex_bonus(pipex);
	}
	else
		res = 255;
	//TODO: implement here_doc
	//fre();
	return (res);
}
