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
	pthread_t		thread;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	log_mutex;
	t_philo			*head;
};

/* Logs */
void		print_log(t_philo *philo, char *action);
int			log_if_philo_alive(t_philo *philo, char *action);
/* Errors */
int			error(char *err_msg, int err);
int			is_nbr(char *nbr);
int			valid_args(int argc, char **argv);
/* Struct */
int			init_sim(t_simulation *sim, char **argv);
int			clean_dllst(t_philo **head, int philo_c);
t_philo		**build_dllst(t_simulation *sim);
/* Threads */
void		*monitor(void *arg);
void		*routine(void *arg);
int			is_philo_died(t_simulation *sim);
/* Utils */
int			destroy_mutexes(t_simulation *sim);
long long	ft_atoll(char *nbr);
void		my_usleep(long time_ms, t_philo *ptr);
long		get_timestamp_ms(void);
#endif