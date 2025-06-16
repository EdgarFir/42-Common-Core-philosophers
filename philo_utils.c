/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edfreder <edfreder@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:16:21 by edfreder          #+#    #+#             */
/*   Updated: 2025/06/16 14:40:37 by edfreder         ###   ########.fr       */
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

long	get_timestamp_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

long long ft_atoll(char *nbr)
{
	long long	result;
	int			signal;
	int			i;
	
	signal = 1;
	result = 0;
	i = 0;
	while ((nbr[i] >= 9 && nbr[i] <= 13) || nbr[i] == 32)
		i++;
	if (nbr[i] == '+' || nbr[i] == '-')
	{
		if (nbr[i] == '-')
			signal = -signal;
		i++;
	}
	while (nbr[i])
	{
		result = result * 10 + (nbr[i] - '0');
		i++;
	}
	return (result);
}

void my_usleep(long time_ms, t_philo *ptr)
{
	long start;
	long current;

	start = get_timestamp_ms();
	current = get_timestamp_ms();
	while (current - start < time_ms)
	{
		pthread_mutex_lock(&ptr->sim->dead_mutex);
		if (ptr->sim->philo_died)
		{
			pthread_mutex_unlock(&ptr->sim->dead_mutex);
			break;
		}
		pthread_mutex_unlock(&ptr->sim->dead_mutex);
		current = get_timestamp_ms();
		usleep(200);
	}
}