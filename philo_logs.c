/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_logs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edfreder <edfreder@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:25:40 by edfreder          #+#    #+#             */
/*   Updated: 2025/06/16 15:14:51 by edfreder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int all_philo_ate(t_simulation *sim)
{
	pthread_mutex_lock(&sim->all_ate_mutex);
	if (sim->all_ate == 1)
	{
		pthread_mutex_unlock(&sim->all_ate_mutex);
		return (1);
	}
	pthread_mutex_unlock(&sim->all_ate_mutex);
	return (0);
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

int	simulation_end(t_simulation *sim)
{
	if (all_philo_ate(sim) || is_philo_died(sim))
		return (1);
	return (0);
}

int log_if_sim_not_end(t_philo *philo, char *action)
{
	if (simulation_end(philo->sim))
		return (0);
	print_log(philo, action);
	return (1);
}

void print_log(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->sim->log_mutex);
	printf("%li %i %s\n", get_timestamp_ms() - philo->sim->time_sim_start,
	philo->id, action);
	pthread_mutex_unlock(&philo->sim->log_mutex);
}