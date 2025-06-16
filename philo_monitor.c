/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edfreder <edfreder@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:23:26 by edfreder          #+#    #+#             */
/*   Updated: 2025/06/16 15:36:59 by edfreder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	philo_ate_n_times(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	if (philo->sim->must_eat_times == philo->eaten_times)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->meal_mutex);
	return (0);
}	

static int	check_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->last_meal_mutex);
	if (get_timestamp_ms() - philo->time_last_meal > philo->sim->time_to_die)
	{
		pthread_mutex_lock(&philo->sim->dead_mutex);
		philo->sim->philo_died = 1;
		print_log(philo, "\033[0;31mdied\033[0m");
		pthread_mutex_unlock(&philo->sim->dead_mutex);
		pthread_mutex_unlock(&philo->last_meal_mutex);
		return (0);
	}
	pthread_mutex_unlock(&philo->last_meal_mutex);
	return (1);
}

static int	check_if_all_ate(t_simulation *sim, int philo_c, int philo_that_ate_n_times)
{
	if (philo_c == philo_that_ate_n_times)
	{
		pthread_mutex_lock(&sim->all_ate_mutex);
		sim->all_ate = 1;
		pthread_mutex_unlock(&sim->all_ate_mutex);
		return (1);
	}
	return (0);
}

void	*monitor(void *arg)
{
	t_simulation	*sim;
	t_philo			*philo;
	int				i;
	int				philo_ate_n_times_c;

	sim = (t_simulation *)arg;
	while (!simulation_end(sim))
	{
		i = 0;
		philo_ate_n_times_c = 0;
		philo = sim->head;
		while (i < sim->philo_c)
		{
			if (!check_last_meal(philo))
				return (NULL);
			if (philo_ate_n_times(philo))
				philo_ate_n_times_c++;
			i++;
			philo = philo->next;
		}
		if (check_if_all_ate(sim, sim->philo_c, philo_ate_n_times_c))
			return (NULL);
		my_usleep(1, philo);
	}
	return (NULL);
}
