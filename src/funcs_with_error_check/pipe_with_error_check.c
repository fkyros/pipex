/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_with_error_check.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 11:17:51 by gade-oli          #+#    #+#             */
/*   Updated: 2023/09/29 21:57:08 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/pipex.h"

void	pipe_with_error_check(int fd[2])
{
	int	res;

	res = pipe(fd);
	if (res == -1)
	{
		perror("pipe failed: ");
		exit(FAIL);
	}
}
