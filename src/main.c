/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 21:54:51 by gade-oli          #+#    #+#             */
/*   Updated: 2023/09/29 21:54:52 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void    fre(void)
{
    system("leaks -q pipex");
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
		res = pipex_logic(pipex);
	}
	else
		res = 255;
	//TODO: implement here_doc
	//fre();
	return (res);
}
