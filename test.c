#include "philo.h"

int sum = 0;
pthread_mutex_t mutex;

void *routine(void *arg)
{
	pthread_mutex_lock(&mutex);
	for (int i = 0; i < 100000; i++)
		sum++;
	pthread_mutex_unlock(&mutex);
	return (NULL);
}

int main()
{
	pthread_t t1, t2;

	pthread_mutex_init(&mutex, NULL);
	pthread_create(&t1, NULL, &routine, NULL);
	pthread_create(&t2, NULL, &routine, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_mutex_destroy(&mutex);

	printf("%i\n", sum);
}