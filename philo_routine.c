/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edfreder <edfreder@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:21:26 by edfreder          #+#    #+#             */
/*   Updated: 2025/06/16 15:37:24 by edfreder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	put_fork_down(pthread_mutex_t *f1, pthread_mutex_t *f2, int status)
{
	if (f1)
		pthread_mutex_unlock(f1);
	if (f2)
		pthread_mutex_unlock(f2);
	return (status);
}

static int think(t_philo *philo)
{
	if (!log_if_sim_not_end(philo, "is thinking"))
		return (0);
	return (1);
}

static int eat(t_philo *philo, pthread_mutex_t *fork1, pthread_mutex_t *fork2)
{
	pthread_mutex_lock(fork1);
	if (!log_if_sim_not_end(philo, "has taken a fork"))
		return (put_fork_down(fork1, NULL, 0));
	pthread_mutex_lock(fork2);
	if (!log_if_sim_not_end(philo, "has taken a fork"))
		return (put_fork_down(fork1, fork2, 0));
	if (!log_if_sim_not_end(philo, "\033[0;32mis eating\033[0m"))
		return (put_fork_down(fork1, fork2, 0));
	pthread_mutex_lock(&philo->meal_mutex);
	philo->eaten_times++;
	pthread_mutex_unlock(&philo->meal_mutex);
	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->time_last_meal = get_timestamp_ms();
	pthread_mutex_unlock(&philo->last_meal_mutex);
	my_usleep(philo->sim->time_to_eat, philo);
	return (put_fork_down(fork1, fork2, 1));
}

void	*routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	while (!simulation_end(philo->sim))
	{
		if (philo->id % 2)
		{
			my_usleep(1, philo);
			if (!eat(philo, &philo->fork, &philo->prev->fork))
				return (NULL);
		}
		else
			if (!eat(philo, &philo->prev->fork, &philo->fork))
				return (NULL);
		if (!think(philo))
			return (NULL);
		if (!log_if_sim_not_end(philo, "is sleeping"))
			return (NULL);
		my_usleep(philo->sim->time_to_sleep, philo);
	}
	return (NULL);
}
