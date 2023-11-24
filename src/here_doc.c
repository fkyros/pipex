/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 21:26:02 by gade-oli          #+#    #+#             */
/*   Updated: 2023/11/24 11:00:01 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

int	here_doc(t_pipex pipex)
{
	char	*str;
	int		fd[2];
	pid_t	pid;

	pipe_with_error_check(fd);
	pid = fork_with_error_check();
	if (pid == 0)
	{
		close(fd[READ_END]);
		while (1)
		{
			ft_putstr_fd("here_doc> ", 1);
			str = get_next_line(STDIN_FILENO);
			str[ft_strlen(str) - 1] = '\0';
			if (!ft_strcmp(str, pipex.here_doc_eof))
				exit(SUCCESS);
			else
				str[ft_strlen(str)] = '\n';
			ft_putstr_fd(str, fd[WRITE_END]);
			free(str);
		}
	}
	waitpid(pid, NULL, 0);
	close(fd[WRITE_END]);
	return (fd[READ_END]);
}
