/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_logs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edfreder <edfreder@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 00:25:30 by edfreder          #+#    #+#             */
/*   Updated: 2025/06/11 14:55:27 by edfreder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int log_if_philo_alive(t_philo *philo, char *action)
{
	if (is_philo_died(philo->sim))
		return (1);
	print_log(philo, action);
	return (0);
}

void print_log(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->sim->log_mutex);
	printf("%li %i %s\n", get_timestamp_ms() - philo->sim->time_sim_start,
	philo->id, action);
	pthread_mutex_unlock(&philo->sim->log_mutex);
}
