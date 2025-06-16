/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edfreder <edfreder@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:10:36 by edfreder          #+#    #+#             */
/*   Updated: 2025/06/16 16:24:20 by edfreder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*single_routine(void *arg)
{
	t_philo	*philo;
	
	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->fork);
	printf("%li %i has taken a fork\n", get_timestamp_ms() - philo->sim->time_sim_start,
	philo->id);
	pthread_mutex_unlock(&philo->fork);
	printf("%li %i died\n", get_timestamp_ms() - philo->sim->time_sim_start,
	philo->id);
	return (NULL);
}

int	sim_single_thread(t_simulation *sim)
{
	t_philo *philo;

	philo = sim->head;
	if (pthread_create(&philo->thread, NULL, &single_routine, philo) != 0)
		return (error("Error: Creating thread.\n", NULL, NULL, 0));
	if (pthread_join(philo->thread, NULL) != 0)
		error("Error: Joining thread.\n", NULL, NULL, 0);
	clean_all(sim, 0);
	return (1);
}

static int  start_sim(t_simulation *sim)
{
	int				i;
	t_philo			*curr;

	if (sim->philo_c > 1)
	{
		if (pthread_create(&sim->thread, NULL, &monitor, sim) != 0)
			return (error("Error: Creathing thread.\n", NULL, NULL, 0));
		i = 0;
		curr = sim->head;
		while (i < sim->philo_c)
		{
			if (pthread_create(&curr->thread, NULL, &routine, curr) != 0)
				return (error("Error: Creathing thread.\n", NULL, NULL, 0));
			i++;
			curr = curr->next;
		}
	}
	else
		return (sim_single_thread(sim));
	return (1);
}

static void finish_sim(t_simulation *sim)
{
	int		i;
	t_philo	*curr;

	if (pthread_join(sim->thread, NULL) != 0)
		error("Error: Joining thread.\n", NULL, NULL, 0);
	i = 0;
	curr = sim->head;
	while (i < sim->philo_c)
	{
		if (pthread_join(curr->thread, NULL) != 0)
			error("Error: Joining thread.\n", NULL, NULL, 0);
		i++;
		curr = curr->next;
	}
	clean_all(sim, 0);
}

int main(int argc, char **argv)
{
    t_simulation    sim;

    if (!valid_args(argc - 1, argv))
		return (1);
	if (!init_sim(&sim, argv))
		return (2);
	if (!build_dllst(&sim))
		return (3);
	if (!start_sim(&sim))
		return (clean_all(&sim, 4));
	if (sim.philo_c > 1)
		finish_sim(&sim);
    return (0);
}