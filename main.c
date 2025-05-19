/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalp <yalp@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 20:42:14 by marvin            #+#    #+#             */
/*   Updated: 2025/05/19 16:26:50 by yalp             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	init_values(t_loop *loop, int i)
{
	loop->philos[i].loop_con = loop;
	loop->philos[i].id = i + 1;
	loop->philos[i].left_fork = &loop->forks[i];
	loop->philos[i].number_of_times_eaten = 0;
	loop->philos[i].last_meal_time = loop->start_time;
	if (i == loop->number_of_philos - 1)
		loop->philos[i].right_fork = &loop->forks[0];
	else
		loop->philos[i].right_fork = &loop->forks[i + 1];
}

void	end(t_loop *loop)
{
	int	i;

	i = 0;
	while (i < loop->number_of_philos)
	{
		pthread_mutex_destroy(&loop->forks[i]);
		i++;
	}
	free(loop->forks);
	free(loop->philos);
}

int	init_args(t_loop *loop, char **argv, int argc)
{
	loop->number_of_philos = ft_atoi(argv[1]);
	loop->time_to_die = ft_atoi(argv[2]);
	loop->time_to_eat = ft_atoi(argv[3]);
	loop->time_to_sleep = ft_atoi(argv[4]);
	loop->start_time = get_time();
	loop->stop = 0;
	loop->eat = 0;
	if (argc == 6)
		loop->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		loop->number_of_times_each_philosopher_must_eat = -1;
	loop->philos = malloc(sizeof(t_philosopher) * loop->number_of_philos);
	if (!loop->philos)
		return (1);
	loop->forks = malloc(sizeof(pthread_mutex_t) * loop->number_of_philos);
	if (!loop->forks)
	{
		free(loop->philos);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_loop	loop;
	int		i;

	if (arg_check(argc, argv) == 1)
		return (1);
	if (init_args(&loop, argv, argc))
		return (1);
	if (init_loop(&loop))
		return (1);
	i = 0;
	pthread_join(loop.control_thread, NULL);
	while (i < loop.number_of_philos)
	{
		pthread_join(loop.philos[i].thread, NULL);
		i++;
	}
	end(&loop);
	return (0);
}
