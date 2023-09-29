/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gade-oli <gade-oli@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 21:54:32 by gade-oli          #+#    #+#             */
/*   Updated: 2023/09/29 21:54:33 by gade-oli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/pipex.h"

/**
 * prints in stderr the message given and returns the failure code
 */
int	ft_error(char *msg)
{
	ft_putstr_fd(msg, 2);
	return (FAIL);
}
