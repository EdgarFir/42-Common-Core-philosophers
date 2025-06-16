/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_struct_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edfreder <edfreder@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:15:28 by edfreder          #+#    #+#             */
/*   Updated: 2025/06/16 15:39:07 by edfreder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*lstnew(int id)
{
	t_philo	*new;

	new = malloc(sizeof(t_philo));
	if (!new)
		return (NULL);
	new->id = id;
	new->next = NULL;
	new->prev = NULL;
	if (pthread_mutex_init(&new->fork, NULL) != 0)
		return (NULL);
	if (pthread_mutex_init(&new->last_meal_mutex, NULL) != 0)
		return (NULL);
	if (pthread_mutex_init(&new->meal_mutex, NULL) != 0)
		return (NULL);
	return (new);
}

void	add_back(t_philo **head, t_philo *new)
{
	t_philo *curr;

	if (!(*head))
		*head = new;
	else
	{
		curr = *head;
		while (curr->next)
			curr = curr->next;
		curr->next = new;
		new->prev = curr;
	}
}
t_philo	**build_dllst(t_simulation *sim)
{
	int		i;
	t_philo	*new;

	i = 0;
	while (i < sim->philo_c)
	{
		new = lstnew(i + 1);
		if (!new)
		{
			clean_dllst(&sim->head, sim->philo_c, 1);
			return (NULL);
		}
		new->sim = sim;
		new->time_last_meal = sim->time_sim_start;
		new->eaten_times = 0;
		add_back(&sim->head, new);
		i++;
	}
	if (new)
	{
		new->next = sim->head;
		sim->head->prev = new;
	}
	return ((t_philo **)sim->head);
}

int	init_sim(t_simulation *sim, char **argv)
{
	sim->philo_c = ft_atoll(argv[1]);
	sim->time_to_die = ft_atoll(argv[2]);
	sim->time_to_eat = ft_atoll(argv[3]);
	sim->time_to_sleep = ft_atoll(argv[4]);
	if (argv[5])
		sim->must_eat_times = ft_atoll(argv[5]);
	else
		sim->must_eat_times = -1;
	if (sim->philo_c == 0 || sim->time_to_die == 0 ||
		sim->time_to_eat == 0 || sim->time_to_sleep == 0 ||
		sim->must_eat_times == 0)
		return (error("Error: Arguments values must be higher than 0.\n", NULL, NULL, 0));
	sim->time_sim_start = get_timestamp_ms();
	if (pthread_mutex_init(&sim->dead_mutex, NULL) != 0)
		return (error("Error: Error initiating mutex.\n", NULL, NULL, 0));
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
		return (error("Error: Error initiating mutex.\n", &sim->dead_mutex, NULL, 0));
	if (pthread_mutex_init(&sim->all_ate_mutex, NULL) != 0)
		return (error("Error: Error initiating all_ate mutex.\n", &sim->dead_mutex, NULL, 0));
	sim->philo_died = 0;
	sim->all_ate = 0;
	sim->head = NULL;
	return (1);
}
