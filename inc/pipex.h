/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 21:10:13 by gade-oli          #+#    #+#             */
/*   Updated: 2023/10/27 17:32:33 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# ifndef BONUS
#  define BONUS 0
# endif

# include "../libft/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>

# define SUCCESS 0
# define FAIL 1

# define ANY_CHILD -1

# define COMMAND_FAIL 127

# define READ_END 0
# define WRITE_END 1

typedef struct s_pipex
{
	char	**argv;
	char	**envp;
	int		ncmds;
	int		here_doc;
	int		fd_to_read_from;
	char	*infile;
	char	*outfile;
	char	*here_doc_eof;
	int		outfile_oflag;
}	t_pipex;

int		pipex_logic(t_pipex pipex);
char	**get_full_command(t_pipex pipex, int ncmd);
int		here_doc(t_pipex pipex);

void	free_command(char **matrix);
int		ft_file_error(char *cmd);
int		ft_error(char *msg);
int		ft_command_error(char *cmd);

void	pipe_with_error_check(int fd[2]);
pid_t	fork_with_error_check(void);

int		get_infile_fd(t_pipex pipex);
int		get_oflags_last_child(t_pipex pipex);
int		get_status_code(t_pipex pipex);

#endif
