/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edfreder <edfreder@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 15:07:37 by edfreder          #+#    #+#             */
/*   Updated: 2025/06/10 00:29:59 by edfreder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <limits.h>

void think(int philo_index)
{

	printf("%li %i is thinking\n", get_timestamp_ms(), philo_index);
}

int	is_philo_died(t_philo *philo)
{
	int	is_died;
	is_died = 0;
	pthread_mutex_lock(&philo->sim->dead_mutex);
	if (*philo->sim->philo_died == 1)
		is_died = 1;
	pthread_mutex_unlock(&philo->sim->dead_mutex);
	return (is_died);
}

void	*routine(void *arg)
{
	t_philo	*ptr;
	pthread_mutex_t *l_fork;
	pthread_mutex_t *r_fork;

	ptr = (t_philo *)arg;
	if (ptr->index % 2)
	{
		l_fork = &ptr->fork;
		r_fork = &ptr->prev->fork;
	}
	else
	{
		l_fork = &ptr->prev->fork;
		r_fork = &ptr->fork;
	}
	while (!is_philo_died(ptr))
	{
		pthread_mutex_lock(l_fork);
		pthread_mutex_lock(&ptr->sim->log_mutex);
		printf("%li %i taken a fork\n", get_timestamp_ms() - ptr->sim->time_sim_start, ptr->index);
		pthread_mutex_unlock(&ptr->sim->log_mutex);
		pthread_mutex_lock(r_fork);
		pthread_mutex_lock(&ptr->sim->log_mutex);
		printf("%li %i taken a fork\n", get_timestamp_ms() - ptr->sim->time_sim_start, ptr->index);
		pthread_mutex_unlock(&ptr->sim->log_mutex);
		pthread_mutex_lock(&ptr->meal_mutex);
		ptr->time_last_meal = get_timestamp_ms();
		pthread_mutex_unlock(&ptr->meal_mutex);
		pthread_mutex_lock(&ptr->sim->log_mutex);
		printf("%li %i is eating\n", get_timestamp_ms() - ptr->sim->time_sim_start, ptr->index);
		pthread_mutex_unlock(&ptr->sim->log_mutex);
		my_usleep(ptr->sim->time_to_eat * 1000, ptr);
		pthread_mutex_unlock(l_fork);
		pthread_mutex_unlock(r_fork);
		pthread_mutex_lock(&ptr->sim->log_mutex);
		think(ptr->index);
		pthread_mutex_unlock(&ptr->sim->log_mutex);
		// Sleep
		pthread_mutex_lock(&ptr->sim->log_mutex);
		printf("%li %i is sleeping\n", get_timestamp_ms() - ptr->sim->time_sim_start, ptr->index);
		pthread_mutex_unlock(&ptr->sim->log_mutex);
		usleep(ptr->sim->time_to_sleep * 1000);
	}
	return (NULL);
}

void *monitor(void *arg)
{
	t_philo *philo;
	t_philo *curr;
	int		i;

	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&philo->sim->dead_mutex);
		if (philo->sim->philo_died)
		{
			pthread_mutex_unlock(&philo->sim->dead_mutex);
			break ;

		}
		pthread_mutex_unlock(&philo->sim->dead_mutex);
		i = 0;
		curr = philo;
		while (i < philo->sim->philo_c)
		{
			long actual_time = get_timestamp_ms();
			pthread_mutex_lock(&curr->meal_mutex);
			long time_diff = actual_time - curr->time_last_meal;
			//printf("Monitor checking philosopher %d: time_since_last_meal=%ld\n",
        	//curr->index, time_diff);
			pthread_mutex_unlock(&curr->meal_mutex);
			if (time_diff > curr->sim->time_to_die)
			{
				pthread_mutex_lock(&philo->sim->dead_mutex);
				*curr->sim->philo_died = 1;
				pthread_mutex_unlock(&philo->sim->dead_mutex);
				pthread_mutex_lock(&philo->sim->log_mutex);
				printf("%li %i died\n", get_timestamp_ms() - curr->sim->time_sim_start, curr->index);
				pthread_mutex_unlock(&philo->sim->log_mutex);
				return (NULL);
			}
			curr = curr->next;
			i++;
		}
		//usleep(1000);
	}
	return (NULL);
}

int main(int argc, char **argv)
{
	t_simulation	sim;
	t_philo			*head;

	if (argc == 1)
		return (1);
	//memset(&sim, 0, sizeof(t_simulation));
	sim.philo_c = ft_atoll(argv[1]);
	sim.time_to_die = ft_atoll(argv[2]);
	sim.time_to_eat = ft_atoll(argv[3]);
	sim.time_to_sleep = ft_atoll(argv[4]);
	*sim.philo_died = 1;
	pthread_mutex_init(&sim.mutex, NULL);
	if (pthread_mutex_init(&sim.dead_mutex, NULL) != 0)
			return (0);
	if (pthread_mutex_init(&sim.log_mutex, NULL) != 0)
			return (0);
	head = NULL;
	if (!build_dllst(&head, &sim))
		return (1);
	init_mutexes(head);
	t_philo *curr = head;
	int i = 0;
	sim.time_sim_start = get_timestamp_ms();
	curr = head;
	while (i < sim.philo_c)
	{
		curr->time_last_meal = sim.time_sim_start;
		curr = curr->next;
		i++;
	}
	curr = head;
	i = 0;
	pthread_create(&head->sim->thread, NULL, &monitor, head);
	while (i < sim.philo_c)
	{
		//curr->time_last_meal = sim.time_sim_start;
		pthread_create(&curr->thread, NULL, &routine, curr);
		curr = curr->next;
		i++;
	}
	curr = head;
	i = 0;
	pthread_join(head->sim->thread, NULL);
	while (i < sim.philo_c)
	{
		pthread_join(curr->thread, NULL);
		curr = curr->next;
		i++;
	}
	//printf("Acabou a simulacao\n");
	//printf("Cleaning\n");
	pthread_mutex_destroy(&head->sim->dead_mutex);
	pthread_mutex_destroy(&head->sim->log_mutex);
	pthread_mutex_destroy(&head->sim->mutex);
	clean_dllst(&head);
	return (0);
}
