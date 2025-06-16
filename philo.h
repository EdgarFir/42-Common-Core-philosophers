/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edfreder <edfreder@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:34:00 by edfreder          #+#    #+#             */
/*   Updated: 2025/06/16 15:33:03 by edfreder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H

# define PHILO_H

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>

typedef struct s_simulation t_simulation;

typedef struct s_philo
{
	pthread_mutex_t		last_meal_mutex;
	pthread_mutex_t		meal_mutex;
	pthread_mutex_t		fork;
	pthread_t			thread;
	long				time_last_meal;
	int					id;
	int					eaten_times;
	struct s_philo		*next;
	struct s_philo 		*prev;
	t_simulation		*sim;
}	t_philo;

struct s_simulation
{
	int				philo_c;
	int				must_eat_times;
	long			time_sim_start;
	long			time_to_eat;
	long			time_to_sleep;
	long			time_to_die;
	int				philo_died;
	int				all_ate;
	pthread_t		thread;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	all_ate_mutex;
	pthread_mutex_t	log_mutex;
	t_philo			*head;
};

/* Logs */
void		print_log(t_philo *philo, char *action);
int			log_if_sim_not_end(t_philo *philo, char *action);
int			simulation_end(t_simulation *sim);
/* Errors */
int			error(char *err_msg, pthread_mutex_t *m1, pthread_mutex_t *m2, int err);
int			valid_args(int argc, char **argv);
/* Struct */
int			init_sim(t_simulation *sim, char **argv);
t_philo		**build_dllst(t_simulation *sim);
/* Utils */
long long	ft_atoll(char *nbr);
void		my_usleep(long time_ms, t_philo *ptr);
long		get_timestamp_ms(void);
int			ft_strlen(char *s);
/* Clean utils */
int			clean_all(t_simulation *sim, int status);
void		clean_dllst(t_philo **head, int philo_c, int destroy_mutexes);
/* Routine */
void	*routine(void *arg);
/* Monitor */
void	*monitor(void *arg);
#endif