/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edfreder <edfreder@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 18:53:55 by edfreder          #+#    #+#             */
/*   Updated: 2025/06/11 15:53:02 by edfreder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int start_sim(t_simulation *sim)
{
	int				i;
	t_philo			*curr;

	if (pthread_create(&sim->thread, NULL, &monitor, sim) != 0)
		return (error("Error: Creathing thread.\n", 0));
	i = 0;
	curr = sim->head;
	while (i < sim->philo_c)
	{
		if (pthread_create(&curr->thread, NULL, &routine, curr) != 0)
			return (error("Error: Creathing thread.\n", 0));
		i++;
		curr = curr->next;
	}
	return (1);
}

int	finish_sim(t_simulation *sim)
{
	int		i;
	t_philo	*curr;

	if (pthread_join(sim->thread, NULL) != 0)
		return (error("Error: Joining thread.\n", 0));
	i = 0;
	curr = sim->head;
	while (i < sim->philo_c)
	{
		if (pthread_join(curr->thread, NULL) != 0)
			return (error("Error: Joining thread.\n", 0));
		i++;
		curr = curr->next;
	}
	destroy_mutexes(sim);
	clean_dllst(&sim->head, sim->philo_c);
	return (1);
}

int main(int argc, char **argv)
{
	t_simulation	sim;

	if (!valid_args(argc - 1, argv))
		return (1);
	if (argc >= 5)
	{
		if (!init_sim(&sim, argv))
			return (2);
		if (!build_dllst(&sim))
			return (3);
		if (!start_sim(&sim))
			return (4);
		if (!finish_sim(&sim))
			return (5);
	}
	return (0);
}