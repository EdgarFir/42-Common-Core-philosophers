/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_clean_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edfreder <edfreder@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:19:23 by edfreder          #+#    #+#             */
/*   Updated: 2025/06/16 16:23:03 by edfreder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_dllst(t_philo **head, int philo_c, int destroy_mutex)
{
	t_philo *tmp;
	t_philo *curr;
	int		i;

	
		curr = *head;
		i = 0;
		while (i < philo_c && curr != NULL)
		{
			tmp = curr->next;
			if (destroy_mutex)
			{
				if (pthread_mutex_destroy(&curr->fork) != 0)
					error("Destroying philo fork mutex.\n", NULL, NULL, 0);
				if (pthread_mutex_destroy(&curr->last_meal_mutex) != 0)
					error("Destroying philo last meal mutex.\n", NULL, NULL, 0);
				if (pthread_mutex_destroy(&curr->meal_mutex) != 0)
					error("Destroying philo meal mutex.\n", NULL, NULL, 0);
			}
			free(curr);
			curr = tmp;
			i++;
		}
		*head = NULL;
	
}

int	destroy_mutexes(t_simulation *sim)
{
	int		i;
	t_philo	*curr;

	if (pthread_mutex_destroy(&sim->dead_mutex) != 0)
		error("Destroying sim dead mutex.\n", NULL, NULL, 0);
	if (pthread_mutex_destroy(&sim->log_mutex) != 0)
		error("Destroying sim log mutex.\n", NULL, NULL, 0);
	if (pthread_mutex_destroy(&sim->all_ate_mutex) != 0)
		error("Destroying all ate mutex.\n", NULL, NULL, 0);

	curr = sim->head;
	i = 0;
	while (i < sim->philo_c)
	{
		if (pthread_mutex_destroy(&curr->fork) != 0)
			error("Destroying philo fork mutex.\n", NULL, NULL, 0);
		if (pthread_mutex_destroy(&curr->last_meal_mutex) != 0)
			error("Destroying philo last meal mutex.\n", NULL, NULL, 0);
		if (pthread_mutex_destroy(&curr->meal_mutex) != 0)
			error("Destroying philo meal mutex.\n", NULL, NULL, 0);
		i++;
		curr = curr->next;
	}
	return (1);
}

int	clean_all(t_simulation *sim, int status)
{
	destroy_mutexes(sim);
	clean_dllst(&sim->head, sim->philo_c, 0);
	return (status);
}
