/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_with_error_check.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 12:07:59 by gade-oli          #+#    #+#             */
/*   Updated: 2023/09/25 12:10:00 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/pipex_bonus.h"

pid_t	fork_with_error_check()
{
	pid_t	res;

	res = fork();
	if (res == -1)
	{
		perror("fork error:");
		exit(FAIL);
	}
	return (res);
}
