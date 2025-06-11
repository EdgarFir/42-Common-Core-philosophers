/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_error_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edfreder <edfreder@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 23:37:21 by edfreder          #+#    #+#             */
/*   Updated: 2025/06/11 15:00:35 by edfreder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(char *s)
{
	int	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

int	is_nbr(char *nbr)
{
	int	i;
	int	is_neg;

	i = 0;
	is_neg = 0;
	if (nbr[i] == '-' || nbr[i] == '+')
	{
		if (nbr[i] == '-')
			is_neg = 1;
		i++;
	}
	while (nbr[i])
	{
		if (nbr[i] != ' ')
		{
			if (nbr[i] < '0' || nbr[i] > '9')
				return (error("Arguments must be numeric.\n", 0));
		}
		i++;
	}
	if (is_neg)
		return (error("Arguments must be positive numbers.\n", 0));
	return (1);
}

int	error(char *err_msg, int err)
{
	write(2, "Error: \n", 9);
	write(2, err_msg, ft_strlen(err_msg));
	return (err);
}

int	valid_args(int argc, char **argv)
{
	int	i;

	if (argc < 4)
		return (error("Invalid number of arguments.\n", 0));
	i = 0;
	while (i < argc)
	{
		if (!(*argv[i + 1]) || !argv[i + 1])
			return (error("Arguments must not be empty.\n", 0));
		if (!is_nbr(argv[i + 1]))
			return (0);
		i++;
	}
	return (1);
}

