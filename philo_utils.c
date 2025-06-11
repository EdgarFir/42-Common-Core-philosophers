/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edfreder <edfreder@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 15:35:51 by edfreder          #+#    #+#             */
/*   Updated: 2025/06/11 15:17:40 by edfreder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	while (1)
	{
		pthread_mutex_lock(&ptr->sim->dead_mutex);
		if (ptr->sim->philo_died)
		{
			pthread_mutex_unlock(&ptr->sim->dead_mutex);
			break;
		}
		pthread_mutex_unlock(&ptr->sim->dead_mutex);

		current = get_timestamp_ms();
		if (current - start >= time_ms)
			break;
		usleep(500);
	}
}

int	destroy_mutexes(t_simulation *sim)
{
	int		i;
	t_philo	*curr;

	if (pthread_mutex_destroy(&sim->dead_mutex) != 0)
		return (error("Destroying sim dead mutex.\n", 0));
	if (pthread_mutex_destroy(&sim->log_mutex) != 0)
		return (error("Destroying sim log mutex.\n", 0));
	curr = sim->head;
	i = 0;
	while (i < sim->philo_c)
	{
		if (pthread_mutex_destroy(&curr->fork) != 0)
		{
			perror("");
			return (error("Destroying philo fork mutex.\n", 0));
		}
		if (pthread_mutex_destroy(&curr->meal_mutex) != 0)
			return (error("Destroying philo meal mutex.\n", 0));
		i++;
		curr = curr->next;
	}
	return (1);
}
