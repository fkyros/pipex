/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 21:54:38 by gade-oli          #+#    #+#             */
/*   Updated: 2023/09/29 21:54:39 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/pipex.h"

/**
 * returns the classic error message from bash as well as the error code
*/
int	ft_file_error(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": no such file or directory", 2);
	return (FAIL);
}
