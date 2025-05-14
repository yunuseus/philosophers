/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 20:42:14 by marvin            #+#    #+#             */
/*   Updated: 2025/05/14 15:53:47 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*start_loop(void	*philosopher)
{
	t_philosopher	philo;

	philo = *(t_philosopher *)philosopher;
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
	return (NULL);
}

void	init_philosophers(t_loop *loop)
{
	int	i;

	i = 0;
	loop->philos = malloc(sizeof(t_philosopher) * loop->number_of_philos);
	while (i < loop->number_of_philos)
	{
		loop->philos[i].loop_con = loop;
		loop->philos[i].id = i + 1;
		loop->philos[i].left_fork = &loop->forks[i];
		loop->philos[i].number_of_times_eaten = 0;
		loop->philos[i].last_meal_time = 0;
		if (i == loop->number_of_philos - 1)
			loop->philos[i].right_fork = &loop->forks[0];
		else
			loop->philos[i].right_fork = &loop->forks[i + 1];
		pthread_create(&loop->philos->thread, NULL,
			start_loop, &loop->philos[i]);
		i++;
	}
}

void	init_loop(t_loop *loop, int argc, char **argv)
{
	int	i;

	i = 0;
	loop->number_of_philos = ft_atoi(argv[1]);
	loop->time_to_die = ft_atoi(argv[2]);
	loop->time_to_eat = ft_atoi(argv[3]);
	loop->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		loop->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		loop->number_of_times_each_philosopher_must_eat = -1;
	loop->forks = malloc(sizeof(pthread_mutex_t) * loop->number_of_philos);
	while (i < loop->number_of_philos)
	{
		pthread_mutex_init(&loop->forks[i], NULL);
		i++;
	}
	init_philosophers(loop);
}

int	main(int argc, char **argv)
{
	t_loop	loop;

	if (arg_check(argc, argv) == 1)
		return (1);
	init_loop(&loop, argc, argv);
	return (0);
}
