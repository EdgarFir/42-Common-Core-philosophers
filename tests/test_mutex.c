#include "philo.h"


void *routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;

	if (philo->index % 2)
	{
		pthread_mutex_lock(&philo->fork);
		printf("%i\n", philo->index);
		// Think
		// Eat
		// Sleep
		pthread_mutex_unlock(&philo->fork);
	}
	else
	{
		pthread_mutex_lock(&philo->fork);
		printf("%i\n", philo->index);
		// Think
		// Eat
		// Sleep
		pthread_mutex_unlock(&philo->fork);
	}
}

t_philo	*lstnew(int index)
{
	t_philo	*new;

	new = malloc(sizeof(t_philo));
	if (!new)
		return (NULL);
	memset(new, 0, sizeof(new));
	new->index = index;
	return (new);
}

void	add_back(t_philo **head, t_philo *new)
{
	t_philo *curr;
	t_philo *prev;

	if (!(*head))
		*head = new;
	else
	{
		curr = *head;
		while (curr->next)
		{
			prev = curr;
			curr = curr->next;
		}
		curr->next = new;
		new->prev = prev;
	}
}

int	init_mutexes(t_philo *head, int n_philo)
{
	int	i;
	t_philo *curr;

	i = 0;
	curr = head;
	while (i < n_philo)
	{
		if (i % 2)
			pthread_mutex_init(&curr->fork, NULL);
		else
			pthread_mutex_init(&curr->fork, NULL);
		i++;
		curr = curr->next;
	}
}
int	create_threads(t_philo *head, int n_philo)
{
	int	i;
	t_philo *curr;

	i = 0;
	curr = head;
	while (i < n_philo)
	{
		printf("index: %i\n", i);
		pthread_create(&curr->thread, NULL, &routine, curr); // Confirmar erro..
		i++;
		curr = curr->next;
	}
}
int main()
{
	t_philo *head;
	t_philo *new;


	// Create lst of structs
	head = NULL;
	for (int i = 0; i < 3; i++)
	{
		new = lstnew(i);
		add_back(&head, new);
		if (i == 2)
		{
			new->next = head;
			head->prev = new;
		}
	}
	printf("%i\n", head->prev->index);
}