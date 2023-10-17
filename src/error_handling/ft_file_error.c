/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 21:54:38 by gade-oli          #+#    #+#             */
/*   Updated: 2023/10/17 20:45:50 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/pipex.h"

/**
 * returns the classic error message from bash as well as the error code
*/
int	ft_file_error(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": permission denied\n", 2);
	return (FAIL);
}
