#include "philo.h"

int main(int argc, char **argv)
{
	struct timeval now;
	struct timeval after;
	

	gettimeofday(&now, NULL);
	long now_sec = now.tv_sec;
	long now_micro_sec = now.tv_usec;

	sleep(5);

	gettimeofday(&after, NULL);
	long after_sec = after.tv_sec;
	long after_micro_sec = after.tv_usec;

	printf("%li\n", (after_sec + (after_micro_sec / 10000)) - (now_sec + (now_micro_sec / 10000)));
}