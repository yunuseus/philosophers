/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 20:42:14 by marvin            #+#    #+#             */
/*   Updated: 2025/05/12 19:29:12 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_digit(char *argv)
{
	while (*argv)
	{
		if (*argv < '0' || *argv > '9')
			return (1);
		argv++;
	}
	return (0);
}

int	is_args_digit(int argc, char **argv)
{
	while (argc > 1)
	{
		if (is_digit(argv[argc - 1]))
		{
			printf("Invalid arguments\n");
			return(1);
		}
		argc--;
	}
}
void	start_loop(void	*philosopher)
{
	philosopher_t philo;
	
	philo = *(philosopher_t *)philosopher;
	while (1)
	{
		if (philo.id % 2 == 0)
		{
			pthread_mutex_lock(philo.left_fork);
			pthread_mutex_lock(philo.right_fork);
		}
		else
		{
			pthread_mutex_lock(philo.right_fork);
			pthread_mutex_lock(philo.left_fork);
		}
		printf("Philosopher %d is eating\n", philo.id);
		usleep(philo.loop_con->time_to_eat * 1000);
	}
	
}
void	init_philosophers(loop_t *loop)
{
	int	i;

	i = 0;
	loop->philosophers = malloc(sizeof(philosopher_t) * loop->number_of_philosophers);
	while (i < loop->number_of_philosophers)
	{
		loop->philosophers[i].loop_con = loop;
		loop->philosophers[i].id = i + 1;
		loop->philosophers[i].left_fork = &loop->forks[i];
		loop->philosophers[i].number_of_times_eaten = 0;
		loop->philosophers[i].last_meal_time = 0;
		if (i == loop->number_of_philosophers - 1)
		loop->philosophers[i].right_fork = &loop->forks[0];
		else
		loop->philosophers[i].right_fork = &loop->forks[i + 1];
		pthread_create(loop->philosophers->thread, NULL, start_loop, &loop->philosophers[i]);
		i++;
	}
}

//ft_atoi
void	init_loop(loop_t *loop, int argc, char **argv)
{
	int	i;

	i = 0;
	loop->number_of_philosophers = atoi(argv[1]);
	loop->time_to_die = atoi(argv[2]);
	loop->time_to_eat = atoi(argv[3]);
	loop->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		loop->number_of_times_each_philosopher_must_eat = atoi(argv[5]);
	else
		loop->number_of_times_each_philosopher_must_eat = -1;
	loop->forks = malloc(sizeof(pthread_mutex_t) * loop->number_of_philosophers);
	while (i < loop->number_of_philosophers)
	{
		pthread_mutex_init(&loop->forks[i], NULL);
		
		i++;
	}
	init_philosophers(loop);
	
}
void	start_loop(loop_t *loop)
{
	if (loop->number_of_times_each_philosopher_must_eat == -1)
	{
		
	}
	else
	{
		
	}
}

int	main(int argc, char **argv)
{
	loop_t	loop;
	if (argc == 5 || argc == 6)
	{
		if (is_args_digit(argc, argv) == 1)
			return(1);
		init_loop(&loop, argc, argv);
		return (0);
	}
	else
	{
		printf("Invalid numbers of arguments\n");
		return(1);
	}
	
}