/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edfreder <edfreder@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 00:22:11 by edfreder          #+#    #+#             */
/*   Updated: 2025/06/11 15:58:10 by edfreder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int eat(t_philo *philo, pthread_mutex_t *fork1, pthread_mutex_t *fork2)
{
	if ((philo->id % 2))
		my_usleep(1, philo);
	pthread_mutex_lock(fork1);
	if (log_if_philo_alive(philo, "taken a fork"))
	{
		pthread_mutex_unlock(fork1);
		return (0);
	}
	pthread_mutex_lock(fork2);
	if (log_if_philo_alive(philo, "taken a fork"))
	{
		pthread_mutex_unlock(fork1);
		pthread_mutex_unlock(fork2);
		return (0);
	}
	if (log_if_philo_alive(philo, "is eating"))
	{
		pthread_mutex_unlock(fork1);
		pthread_mutex_unlock(fork2);
		return (0);
	}
	pthread_mutex_lock(&philo->meal_mutex);
	philo->time_last_meal = get_timestamp_ms();
	pthread_mutex_unlock(&philo->meal_mutex);
	my_usleep(philo->sim->time_to_eat, philo);
	pthread_mutex_unlock(fork1);
	pthread_mutex_unlock(fork2);
	return (1);
}

void	*routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (is_philo_died(philo->sim))
			return (NULL);
		if (philo->id % 2)
		{
			if (!eat(philo, &philo->fork, &philo->prev->fork))
				return (NULL);
		}
		else
			if (!eat(philo, &philo->prev->fork, &philo->fork))
				return (NULL);
		if (log_if_philo_alive(philo, "is thinking"))
			return (NULL);
		if (log_if_philo_alive(philo, "is sleeping"))
			return (NULL);
		my_usleep(philo->sim->time_to_sleep, philo);
	}
	return (NULL);
}

void	*monitor(void *arg)
{
	t_simulation	*sim;
	t_philo			*curr;
	int				i;

	sim = (t_simulation *)arg;
	while (1)
	{
		i = 0;
		curr = sim->head;
		while (i < sim->philo_c)
		{
			pthread_mutex_lock(&curr->meal_mutex);
			if (get_timestamp_ms() - curr->time_last_meal > sim->time_to_die)
			{
				pthread_mutex_lock(&sim->dead_mutex);
				sim->philo_died = 1;
				print_log(curr, "died");
				pthread_mutex_unlock(&sim->dead_mutex);
				pthread_mutex_unlock(&curr->meal_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&curr->meal_mutex);
			i++;
			curr = curr->next;
		}
		my_usleep(1, curr);
	}
	return (NULL);
}

int	is_philo_died(t_simulation *sim)
{
	pthread_mutex_lock(&sim->dead_mutex);
	if (sim->philo_died == 1)
	{
		pthread_mutex_unlock(&sim->dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(&sim->dead_mutex);
	return (0);
}
