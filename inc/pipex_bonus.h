/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 21:10:13 by gade-oli          #+#    #+#             */
/*   Updated: 2023/09/25 12:44:38 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# define BONUS 1 //TODO: dejarlo a cero al finalizar el desarrollo

# include "../libft/libft.h"
# include <unistd.h>
# include <fcntl.h>

# define SUCCESS 0
# define FAIL 1

# define READ_END 0
# define WRITE_END 1

# define COMMAND_FAIL 127 //TODO: confirmar codigo de error

typedef struct s_pipex
{
	char	**argv;
	char	**envp;
	int		ncmds;
	int		here_doc;
	int		fd_to_read_from;
	char	*infile;
	char	*outfile;
}	t_pipex;

void	pipe_with_error_check(int fd[2]);
void	fork_with_error_check();

#endif
