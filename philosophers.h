#ifndef PHILOSOPHERS_H

#define PHILOSOPHERS_H
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct philosopher_s
{
	int		id;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	int					last_meal_time;
	int					number_of_times_eaten;
	pthread_t			thread;
	struct	loop_s		*loop_con;
} philosopher_t;

typedef struct loop_s
{
	philosopher_t	*philosophers;
	int				number_of_philosophers;
	int				start_time;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	pthread_mutex_t	*forks;
	int				number_of_times_each_philosopher_must_eat;
} loop_t;


#endif